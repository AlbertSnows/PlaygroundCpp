#include "order_book.hpp"

std::map<double, std::set<int>> get_order_by_side(const OrderBook& book, Side side) {
    if (side == Side::Buy) {
        return book.buying_orders;
    }
    return book.selling_orders;
}

std::map<double, std::set<int>> get_opposing_orders(const OrderBook& book, Side side) {
    auto opposing_side = side == Side::Buy ? Side::Sell : Side::Buy;
    return get_order_by_side(book, opposing_side);
}

// a trade occurs when the lowest sell price is at or below the highest buy price
bool gap_is_crossed(const Order &order_inquiry, const OrderBook& book) {
    auto opposing_orders = get_opposing_orders(book, order_inquiry.side);
    auto is_buy = order_inquiry.side == Side::Buy;
    if (opposing_orders.empty()) {
        return false;
    }
    auto crossed_buy_gap = (is_buy && opposing_orders.rbegin()->first <= order_inquiry.price);
    auto crossed_sell_gap = (!is_buy && order_inquiry.price <= opposing_orders.begin()->first);
    return crossed_buy_gap || crossed_sell_gap;
}

template<typename T>
std::optional<T> take_first(std::set<T>& collection) {
    if (!collection.empty()) {
        auto item = *collection.begin();
        collection.erase(item);
        return std::optional(item);
    }
    return std::nullopt;
}

template<typename T>
std::optional<T> take_last(std::set<T>& collection) {
    if (!collection.empty()) {
        auto item = *collection.rbegin();
        collection.erase(item);
        return std::optional(item);
    }
    return std::nullopt;
}

// we assume the gap is crossed
// either one or no order is returned, alongside the trade
std::pair<Trade, std::optional<Order>> process_one_order(const Order &buy_order, const Order &sell_order) {
    auto buys_will_remain = buy_order.quantity > sell_order.quantity;
    auto orders_will_cancel = buy_order.quantity == sell_order.quantity;
    if (orders_will_cancel) {
        return std::pair(Trade {
            buy_order.id,
            sell_order.id,
            buy_order.price,
            buy_order.quantity,
        },
                         std::nullopt);
    }

    if (buys_will_remain) {
        return std::pair(Trade {
            buy_order.id,
            sell_order.id,
            buy_order.price,
            buy_order.quantity,
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
                         sell_order.price,
                         sell_order.quantity,
                     }, Order {
                         sell_order.id,
                         Side::Sell,
                         sell_order.price,
                         (sell_order.quantity - buy_order.quantity),
                     });
}

// we know the limit order crosses the gap
std::pair<std::optional<Order>, std::vector<Trade>> transact_limit_order(OrderBook book, Order limit_order) {
    auto opposing_orders = get_opposing_orders(book, limit_order.side);
    auto completed_transactions = std::vector<Trade>();
    while (limit_order.quantity > 0 && gap_is_crossed(limit_order, book)) {
        double best_price = limit_order.side == Side::Sell
            ? opposing_orders.begin()->first
            : opposing_orders.rbegin()->first;
        auto matching_orders = opposing_orders[best_price];
        auto processing_outcome = book.process_orders_at_price(limit_order, matching_orders);
        auto limit_order_state = processing_outcome.first;
        if (limit_order_state != std::nullopt) {
            limit_order = limit_order_state.value();
        } else {
            limit_order.quantity = 0;
        }
        completed_transactions.insert(
            completed_transactions.end(),
            processing_outcome.second.begin(),
            processing_outcome.second.end());
    }
    auto outcome = limit_order.quantity > 0 ? std::optional(limit_order) : std::nullopt;
    return {outcome, completed_transactions};
}

void upsert(std::map<double, std::set<int>> listing, double price, int order_id) {
    auto orders_exist_at_price = listing.find(price) != listing.end();
    if (orders_exist_at_price) {
        auto orders = listing.at(price);
        orders.insert(order_id);
    } else {
        listing[price] = std::set{order_id};
    }
}

std::vector<Trade> addLimitOrder(OrderBook book, Order limit_order) {
    // auto is_buy = limit_order.side == Side::Buy;
    auto opposing_orders = get_opposing_orders(book, limit_order.side);
    auto should_transact = gap_is_crossed(limit_order, book);
    auto completed_transactions = std::vector<Trade>();
    if (should_transact) {
        auto transact_outcome = transact_limit_order(book, limit_order);
        auto remaining_order = transact_outcome.first;
        if (remaining_order.has_value()) {
            limit_order = remaining_order.value();
        }
        completed_transactions.insert(
            completed_transactions.begin(),
            transact_outcome.second.begin(),
            transact_outcome.second.end());
    }
    book.order_index.insert_or_assign(limit_order.id, limit_order);
    auto corresponding_orders = get_order_by_side(book, limit_order.side);

    upsert(corresponding_orders, limit_order.price, limit_order.id);
    book.order_index.insert_or_assign(limit_order.id, limit_order);
    return completed_transactions;
}

std::pair<std::optional<Order>, std::vector<Trade>> OrderBook::process_orders_at_price(Order limit_order, std::set<int> matching_orders) {
    auto completed_transactions = std::vector<Trade>();
    while (limit_order.quantity > 0 && !matching_orders.empty()) {
        auto take_closest = limit_order.side == Side::Buy ? take_last<int> : take_first<int>;
        auto matching_id = take_closest(matching_orders);
        if (matching_id == std::nullopt) {
            break;
        }
        Order matching_order = order_index.at(matching_id.value());
        auto transaction_outcome = process_one_order(limit_order, matching_order);
        auto cancelled_out = transaction_outcome.second == std::nullopt;
        auto limit_orders_remain = !cancelled_out && transaction_outcome.second->side == limit_order.side;
        if (cancelled_out) {
            limit_order.quantity = 0;
            cancelOrder(matching_order.id);
        } else if (limit_orders_remain) {
            limit_order = transaction_outcome.second.value();
            cancelOrder(matching_order.id);
        } else if (matching_orders.empty()) {
            limit_order = transaction_outcome.second.value();
            cancelPrice(matching_order.price, matching_order.side);
        } else { // matching order remains
            updateOrder(transaction_outcome.second.value());
        }
        // trade_history.push_back(transaction_outcome.first);
        completed_transactions.push_back(transaction_outcome.first);
    }
    std::optional<Order> outcome = (limit_order.quantity > 0)
        ? std::optional(limit_order)
        : std::nullopt;
    trade_history.insert(trade_history.end(), completed_transactions.begin(), completed_transactions.end());
    return {outcome, completed_transactions};
}

Order OrderBook::updateOrder(const Order &order) {
    order_index.at(order.id) = order;
    return order;
}

bool OrderBook::cancelPrice(double price, const Side side) {
    auto relevant_listing = get_order_by_side(*this, side);
    auto orders = relevant_listing[price];
    if (orders.empty()) {
        return false;
    }
    for (int order_id : orders) {
        cancelOrder(order_id);
    }
    relevant_listing.erase(price);
    return true;
}

bool OrderBook::cancelOrder(int order_id) {
    std::optional maybe_order = order_index.at(order_id);
    if (!maybe_order.has_value()) {
        return false;
    }
    auto order_type = maybe_order.value().side;
    auto order_price = maybe_order.value().price;
    if (order_type == Side::Buy) {
        auto orders_at_price = buying_orders[order_price];
        orders_at_price.erase(order_id);
    } else {
        auto orders_at_price = selling_orders[order_price];
        orders_at_price.erase(order_id);
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