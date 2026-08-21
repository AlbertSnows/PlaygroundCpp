#include <iostream>
#include "order_book.hpp"
using namespace std;

void expect(bool condition, const string& label) {
    cout << (condition ? "PASS" : "FAIL") << " — " << label << "\n";
}

void test_restingOrderNoCross() {
    OrderBook book;
    auto trades = addLimitOrder(book, {1, Side::Buy, 100.0, 10});
    expect(trades.empty(), "resting buy generates no trades");
    // std::cout << book.bestBid().has_value() << "\n";
    expect(book.bestBid() == 100.0, "bestBid reflects the resting order");
    expect(!book.bestAsk().has_value(), "bestAsk is empty, no sell orders yet");
}

void test_crossingOrderFullyFills() {
    OrderBook book;
    addLimitOrder(book, {1, Side::Buy, 100.0, 10});
    auto trades = addLimitOrder(book, {2, Side::Sell, 100.0, 10});
    expect(trades.size() == 1, "crossing sell generates exactly one trade");
    if (!trades.empty()) {
        auto& t = trades[0];
        expect(t.buy_order_id == 1 && t.sell_order_id == 2 && t.quantity == 10,
               "trade matches buy=1, sell=2, quantity=10");
    }
    expect(!book.bestBid().has_value(), "book empty after full fill: no bid");
    expect(!book.bestAsk().has_value(), "book empty after full fill: no ask");
}

void test_partialFillRests() {
    OrderBook book;
    addLimitOrder(book, {1, Side::Sell, 50.0, 5});
    auto trades = addLimitOrder(book, {2, Side::Buy, 50.0, 8});
    expect(trades.size() == 1 && trades[0].quantity == 5,
           "partial fill: one trade for the resting order's full quantity (5)");
    expect(book.bestBid() == 50.0, "remaining 3 of the buy order rests");
    expect(!book.bestAsk().has_value(), "resting sell fully consumed");
}

void test_priceTimePriority() {
    OrderBook book;
    addLimitOrder(book, {1, Side::Buy, 100.0, 5});   // arrives first
    addLimitOrder(book, {2, Side::Buy, 100.0, 5});   // same price, arrives second
    auto trades = addLimitOrder(book, {3, Side::Sell, 100.0, 5});
    expect(trades.size() == 1 && trades[0].buy_order_id == 1,
           "at equal price, the earlier-arrived order (id=1) fills first");
    expect(book.bestBid() == 100.0, "order 2's quantity still rests");
}

void test_cancelOrder() {
    OrderBook book;
    addLimitOrder(book, {1, Side::Buy, 75.0, 10});
    expect(book.cancelOrder(1), "cancelling a resting order returns true");
    expect(!book.bestBid().has_value(), "book empty after cancelling the only order");
    expect(!book.cancelOrder(999), "cancelling a nonexistent id returns false");
}

int main() {
    vector test_list = {
        test_restingOrderNoCross,
        // test_crossingOrderFullyFills,
        // test_partialFillRests,
        // test_priceTimePriority,
        // test_cancelOrder
    };
    for (auto test_num = 0; test_num < static_cast<int>(test_list.size()); test_num++) {
        cout << "test " << test_num << "\n";
        test_list[test_num]();
    }
    return 0;
}
