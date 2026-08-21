#include "order_book.hpp"

const std::map<double, std::set<int>>& OrderBook::get_order_by_side(const Side side) const {
    return side == Side::Buy ? buying_orders : selling_orders;
}


const std::map<double, std::set<int>>& OrderBook::get_opposing_orders(const Side side) const {
    const auto opposing_side = side == Side::Buy ? Side::Sell : Side::Buy;
    return get_order_by_side(opposing_side);
}

template<typename T>
T& make_writable(const T& ref) {
    return const_cast<T&>(ref);
}

std::vector<Trade> OrderBook::addLimitOrder(OrderBook& book, Order limit_order) {
    // auto is_buy = limit_order.side == Side::Buy;
    const auto opposing_orders = get_opposing_orders(limit_order.side);
    const bool should_transact = gap_is_crossed(limit_order);
    auto transactions_for_history = std::vector<Trade>();
    if (should_transact) {
        const auto [maybe_remaining_limit_order, completed_transactions] =
            transact_limit_order(limit_order);
        const auto remaining_order = maybe_remaining_limit_order;
        if (remaining_order.has_value()) {
            limit_order = remaining_order.value();
        } else {
            limit_order.quantity = 0;
        }
        transactions_for_history.insert(
            transactions_for_history.end(),
            completed_transactions.begin(),
            completed_transactions.end());
    }

    if (limit_order.quantity > 0) {
        auto& corresponding_orders = make_writable(get_order_by_side(limit_order.side));
        upsert(corresponding_orders, limit_order.price, limit_order.id);
        book.order_index.insert_or_assign(limit_order.id, limit_order);
    }
    return transactions_for_history;
}


// a trade occurs when the lowest sell price is at or below the highest buy price
bool OrderBook::gap_is_crossed(const Order &order_inquiry) const {
    const auto opposing_orders = get_opposing_orders(order_inquiry.side);
    const auto is_buy = order_inquiry.side == Side::Buy;
    if (opposing_orders.empty()) {
        return false;
    }
    const auto crossed_buy_gap = (is_buy
            && bestAsk().has_value()
            && bestAsk().value() >= order_inquiry.price);
    const auto crossed_sell_gap = (!is_buy
            && bestBid().has_value()
            && order_inquiry.price <= bestBid().value());
    return crossed_buy_gap || crossed_sell_gap;
}

std::optional<double> get_best_resting_price(Side side, const std::map<double, std::set<int>>& opposing_orders) {
    if (opposing_orders.empty()) {
        return std::nullopt;
    }
    auto order_is_buy = side == Side::Buy;
    auto best_price = order_is_buy // want "best" sell
        ? opposing_orders.begin()->first
        : opposing_orders.rbegin()->first;
    return best_price;
}

// we know the limit order crosses the gap
std::pair<std::optional<Order>, std::vector<Trade>> OrderBook::transact_limit_order(
    Order limit_order) {
    const auto& opposing_orders = get_opposing_orders(limit_order.side);
    auto completed_transactions = std::vector<Trade>();
    while (limit_order.quantity > 0 && gap_is_crossed(limit_order)) {
        const double best_price = get_best_resting_price(limit_order.side, opposing_orders).value();
        auto& matching_orders = make_writable(opposing_orders.at(best_price));
        const auto [maybe_limit_order, transactions_at_price] =
            process_orders_at_price(limit_order, matching_orders);
        if (maybe_limit_order != std::nullopt) {
            limit_order = maybe_limit_order.value();
        } else {
            limit_order.quantity = 0;
        }
        completed_transactions.insert(
            completed_transactions.end(),
            transactions_at_price.begin(),
            transactions_at_price.end());
    }
    const auto outcome = limit_order.quantity > 0
        ? std::optional(limit_order)
        : std::nullopt;
    return {outcome, completed_transactions};
}


std::pair<std::optional<Order>, std::vector<Trade>> OrderBook::process_orders_at_price(
    const Order& limit_order,
    std::set<int>& matching_orders) {
    auto limit_order_state = limit_order;
    auto completed_transactions = std::vector<Trade>();
    while (limit_order_state.quantity > 0 && !matching_orders.empty()) {
        const auto matching_id = get_closest_id(limit_order.side, matching_orders);
        if (matching_id == std::nullopt) {
            break;
        }
        const Order matching_order = order_index.at(matching_id.value());
        const auto [trade, remaining_order] = process_one_order(limit_order, matching_order);
        const auto cancelled_out = remaining_order == std::nullopt;
        const auto limit_orders_remain = !cancelled_out
            && remaining_order->id == limit_order.id;

        // limit order cases
        // if limit order remains -> update it
        // else -> set it to 0
        if (limit_orders_remain) {
            limit_order_state = remaining_order.value();
        } else {
            limit_order_state.quantity = 0;
        }

        // matching order cases
        // matching order remains -> update it
        // else -> remove it
        const auto matching_order_remains = !cancelled_out
            && remaining_order->id == matching_order.id;
        if (matching_order_remains) {
            updateOrder(remaining_order.value());
        } else {
            cancelOrder(matching_order.id);
        }

        // price cases
        /// was_last_matching_order ?
        /// if no matching order remains -> remove price
        /// else -> nothing
        // const auto was_last_matching_order = matching_orders.empty();
        // if (was_last_matching_order & !matching_order_remains) {
        //     cancelPrice(matching_order.price, matching_order.side);
        // }
        completed_transactions.push_back(trade);
    }

    // ledger recorded at the same time orders are updated
    trade_history.insert(trade_history.end(), completed_transactions.begin(), completed_transactions.end());
    const std::optional<Order> outcome = (limit_order_state.quantity > 0)
        ? std::optional(limit_order_state)
        : std::nullopt;
    return {outcome, completed_transactions};
}

std::optional<Order> OrderBook::updateOrder(const Order &order) {
    if (order_index.find(order.id) != order_index.end()) {
        order_index.at(order.id) = order;
        return order;
    }
    return std::nullopt;
}

bool OrderBook::cancelPrice(const double price, const Side side) {
    auto relevant_listing = get_order_by_side(side);
    const auto orders = relevant_listing[price];
    if (orders.empty()) {
        return false;
    }
    for (const int order_id : orders) {
        cancelOrder(order_id);
    }
    relevant_listing.erase(price);
    return true;
}

bool OrderBook::cancelOrder(const int order_id) {
    const std::optional maybe_order = order_index.find(order_id);
    if (maybe_order == order_index.end()) {
        return false;
    }
    const auto order = order_index.at(order_id);
    const auto order_type = order.side;
    const auto order_price = order.price;
    if (order_type == Side::Buy) {
        auto& orders_at_price = buying_orders[order_price];
        orders_at_price.erase(order_id);
        if (orders_at_price.empty()) {
            buying_orders.erase(order_price);
        }
    } else {
        auto& orders_at_price = selling_orders[order_price];
        orders_at_price.erase(order_id);
        if (orders_at_price.empty()) {
            selling_orders.erase(order_price);
        }
    }
    order_index.erase(order_id);
    return true;
}

std::optional<double> OrderBook::bestBid() const {
    if (buying_orders.empty()) {
        return std::nullopt;
    }
    return buying_orders.rbegin()->first;
}

std::optional<double> OrderBook::bestAsk() const {
    if (selling_orders.empty()) {
        return std::nullopt;
    }
    return selling_orders.begin()->first;
}

// -------------------


std::optional<int> get_closest_id(
    const Side side,
    const std::set<int>& matching_orders) {
    if (matching_orders.empty()) return std::nullopt;
    return side == Side::Buy
        ? *matching_orders.rbegin()
        : *matching_orders.begin();
}

// we assume the gap is crossed
// either one or no order is returned, alongside the trade
std::pair<Trade, std::optional<Order>> process_one_order(
    const Order &limit_order,
    const Order &matching_order) {
    const auto& buy_order = limit_order.side == Side::Buy ? limit_order : matching_order;
    const auto& sell_order = limit_order.side == Side::Sell ? limit_order : matching_order;
    const auto buys_will_remain = buy_order.quantity > sell_order.quantity;
    const auto orders_will_cancel = buy_order.quantity == sell_order.quantity;
    if (orders_will_cancel) {
        return std::pair(Trade {
            buy_order.id,
            sell_order.id,
            matching_order.price,
            buy_order.quantity,
        },
        std::nullopt);
    }

    if (buys_will_remain) {
        return std::pair(Trade {
            buy_order.id,
            sell_order.id,
            matching_order.price,
            sell_order.quantity,
        }, Order {
            buy_order.id,
            Side::Buy,
            buy_order.price,
            (buy_order.quantity - sell_order.quantity),
        });
    }
    // sells remain
    return std::pair(Trade {
                         buy_order.id,
                         sell_order.id,
                         matching_order.price,
                         buy_order.quantity,
                     }, Order {
                         sell_order.id,
                         Side::Sell,
                         sell_order.price,
                         (sell_order.quantity - buy_order.quantity),
                     });
}

template<typename Map>
void upsert(Map& mapping, typename Map::key_type key, typename Map::mapped_type::value_type value) {

    const auto key_exists = mapping.find(key) != mapping.end();
    if (key_exists) {
        auto& collection = mapping.at(key);
        collection.insert(value);
    } else {
        mapping[key] = std::set{value};
    }
}

// template<typename T>
// std::optional<T> take_first(std::set<T>& collection) {
//     if (!collection.empty()) {
//         auto item = *collection.begin();
//         collection.erase(item);
//         return std::optional(item);
//     }
//     return std::nullopt;
// }
//
// template<typename T>
// std::optional<T> take_last(std::set<T>& collection) {
//     if (!collection.empty()) {
//         auto item = *collection.rbegin();
//         collection.erase(item);
//         return std::optional(item);
//     }
//     return std::nullopt;
// }
