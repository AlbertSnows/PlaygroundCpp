#include "order_book.hpp"

// TODO: implement OrderBook::addLimitOrder, cancelOrder, bestBid, bestAsk
// here. See order_book.hpp for the contract each needs to satisfy.

void OrderBook::addLimitOrder(Order limit_order) {
    // auto is_buy = limit_order.side == Side::Buy;
    auto opposing_orders = get_opposing_orders(self, limit_order.side);
    auto should_transact = gap_is_crossed(self, limit_order);
    if (should_transact) {
        auto remaining_order = transact_limit_order(limit_order);
        if (remaining_order.has_value()) {
            limit_order = remaining_order.value();
        }
    }
    self.order_index[limit_order.id] = limit_order;
    auto corresponding_orders = limit_order.side == Side::Buy ? self.buying_book : self.selling_book;
    corresponding_orders.upsert(limit_order.price, limit_order.id);

}

bool get_opposing_orders(const OrderBook& book, Side side) {
    return side == Side::Buy ? book.buying_book : book.selling_book;
}

bool OrderBook::updateOrder(Order order) {
    self.order_index[order.id] = order;
}

bool OrderBook::cancelPrice(int price, Side side) {
    auto book = side == Side::Buy ? self.buying_book : selling_book;
    auto maybe_entry = book[price];
    if (!(maybe_entry.has_value())) {
        return false;
    }
    for (Order order : maybe_entry.value()->second) {
        cancelOrder(order.id);
    }
    book.remove(price);
}

bool OrderBook::cancelOrder(int order_id) {
    std::optional<Order> maybe_order = self.order_index.find(order_id);
    if (maybe_order.has_value()) {
        auto order_type = maybe_order.value().side;
        auto relevant_book = order_type == Side::Buy ? self.buying_book : self.selling_book;
        auto order_price = maybe_order.value().price;
        auto book_entry = self.relevant_book[order_price];
        auto orders_at_price = book_entry->second;
        orders_at_price.erase(order_id);
        self.order_index.remove(order_id);
    }
    return maybe_order.has_value();
}

std::optional<double> OrderBook::bestBid() {
    auto maybe_best_bid_entry = self.buying_book.begin();
    if (maybe_best_bid_entry == null) {
        return nullopt;
    }
    return maybe_best_bid_entry->first;
}

std::optional<double> OrderBook::bestAsk() {
    auto maybe_best_ask_entry = self.selling_book.begin();
    if (maybe_best_ask_entry == null) {
        return nullopt;
    }
    return maybe_best_ask_entry->first;
}

// a trade occurs when the lowest sell price is at or below the highest buy price
bool gap_is_crossed(Order order_inquiry, OrderBook book) {
    auto opposing_orders = get_opposing_orders(book, order_inquiry.side);
    auto is_buy = order_inquiry.side == Side::Buy;
    return (is_buy && opposing_orders.begin() <= order_inquiry.price) ||
        order_inquiry.price <= opposing_orders.begin(); // && is_sell

}

// we know the limit order crosses the gap
std::optional<Order> OrderBook::transact_limit_order(Order limit_order) {
    auto opposing_orders = get_opposing_orders(self, limit_order.side);
    while (limit_order.quantity > 0 && gap_is_crossed(limit_order, self)) {
        auto matching_orders = opposing_orders.begin();
        auto processing_outcome = process_orders_at_price(limit_order, matching_orders);
        if (processing_outcome != nullopt) {
            limit_order = processing_outcome;
        } else {
            limit_order.quantity = 0;
        }
    }
    auto outcome = limit_order.quantity > 0 ? limit_order : nullopt;
    return outcome;
}

std::optional<Order> OrderBook::process_orders_at_price(const Order limit_order, const std::pmr::deque<Order> matching_orders) {
    while (limit_order.quantity > 0 && !matching_orders.empty()) {
        auto matching_order = matching_orders.take();
        if (matching_order == null) {
            break;
        }
        auto transaction_outcome = process_one_order(limit_order, matching_order);
        self.trade_history.push_back(transaction_outcome.first);
        auto cancelled_out = transaction_outcome.second == nullopt;
        auto limit_orders_remain = !cancelled_out && transaction_outcome->second.side == limit_order.side;
        if (cancelled_out) {
            limit_order.quantity = 0;
            cancelOrder(matching_order.id);
        } else if (limit_orders_remain) {
            limit_order = transaction_outcome->second;
            cancelOrder(matching_order.id);
        } else if (matching_orders.empty()) {
            cancelPrice(matching_order.price);
        } else { // matching order remains
            updateOrder(transaction_outcome->second);
        }

    }
    auto outcome = limit_order.quantity > 0 ? limit_order : nullopt;
    return outcome;
}

// we assume the gap is crossed
// either one or no order is returned, alongside the trade
std::pair<Trade, std::optional<Order>> process_one_order(const Order &buy_order, const Order &sell_order) {
    auto buys_will_remain = buy_order.quantity > sell_order.quantity;
    auto orders_will_cancel = buy_order.quantity == sell_order.quantity;
    if (orders_will_cancel) {
        return std::pair(Trade {
            buy_order_id: buy_order.id,
            sell_order_id: sell_order.id,
            price: buy_order.price,
            quantity: buy_order.quantity,
        },
        nullopt);
    }

    if (buys_will_remain) {
        return std::pair(Trade {
            buy_order_id: buy_order.id,
            sell_order_id: sell_order.id,
            price: buy_order.price,
            quantity: buy_order.quantity,
        }, Order {
            id: buy_order.id,
            side: Side::Buy,
            price: buy_order.price,
            quantity: (buy_order.quantity - sell_order.quantity),
        });
    }
    // sells remain
    return std::pair(Trade {
                         buy_order_id: buy_order.id,
                         sell_order_id: sell_order.id,
                         price: sell_order.price,
                         quantity: sell_order.quantity,
                     }, Order {
                         id: sell_order.id,
                         side: Side::Sell,
                         price: sell_order.price,
                         quantity: (sell_order.quantity - buy_order.quantity),
                     });
}
