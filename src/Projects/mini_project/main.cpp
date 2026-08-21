#include <iostream>
#include "order_book.hpp"
using namespace std;

void expect(bool condition, const string& label) {
    cout << (condition ? "PASS" : "FAIL") << " — " << label << "\n";
}

void test_restingOrderNoCross() {
    OrderBook book;
    auto trades = book.addLimitOrder(book, {1, Side::Buy, 100.0, 10});
    expect(trades.empty(), "resting buy generates no trades");
    // std::cout << book.bestBid().has_value() << "\n";
    expect(book.bestBid() == 100.0, "bestBid reflects the resting order");
    expect(!book.bestAsk().has_value(), "bestAsk is empty, no sell orders yet");
}

void test_crossingOrderFullyFills() {
    OrderBook book;
    book.addLimitOrder(book, {1, Side::Buy, 100.0, 10});
    auto trades = book.addLimitOrder(book, {2, Side::Sell, 100.0, 10});
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
    book.addLimitOrder(book, {1, Side::Sell, 50.0, 5});
    auto trades = book.addLimitOrder(book, {2, Side::Buy, 50.0, 8});
    expect(trades.size() == 1 && trades[0].quantity == 5,
           "partial fill: one trade for the resting order's full quantity (5)");
    expect(book.bestBid() == 50.0, "remaining 3 of the buy order rests");
    expect(!book.bestAsk().has_value(), "resting sell fully consumed");
}

void test_priceTimePriority() {
    OrderBook book;
    book.addLimitOrder(book, {1, Side::Buy, 100.0, 5});   // arrives first
    book.addLimitOrder(book, {2, Side::Buy, 100.0, 5});   // same price, arrives second
    auto trades = book.addLimitOrder(book, {3, Side::Sell, 100.0, 5});
    expect(trades.size() == 1 && trades[0].buy_order_id == 1,
           "at equal price, the earlier-arrived order (id=1) fills first");
    expect(book.bestBid() == 100.0, "order 2's quantity still rests");
}

void test_cancelOrder() {
    OrderBook book;
    book.addLimitOrder(book, {1, Side::Buy, 75.0, 10});
    expect(book.cancelOrder(1), "cancelling a resting order returns true");
    expect(!book.bestBid().has_value(), "book empty after cancelling the only order");
    expect(!book.cancelOrder(999), "cancelling a nonexistent id returns false");
}

void test_addThenCancel() {
    OrderBook book;
    book.addLimitOrder(book, {1, Side::Buy, 60.0, 10});
    expect(book.cancelOrder(1), "cancelling the resting order returns true");
    expect(!book.bestBid().has_value(), "book empty after cancelling the only resting order");
}

void test_manyOrdersMixedTransactions() {
    OrderBook book;
    book.addLimitOrder(book, {1, Side::Buy, 100.0, 5});   // rests
    book.addLimitOrder(book, {2, Side::Buy, 99.0, 5});    // rests
    book.addLimitOrder(book, {3, Side::Sell, 105.0, 5});  // rests, no cross (bestBid 100 < 105)
    book.addLimitOrder(book, {4, Side::Sell, 106.0, 5});  // rests

    auto trades5 = book.addLimitOrder(book, {5, Side::Buy, 105.0, 5});  // crosses order 3, fully
    expect(trades5.size() == 1 && trades5[0].sell_order_id == 3 && trades5[0].quantity == 5,
           "order 5 fully fills against resting sell order 3 at 105");

    auto trades6 = book.addLimitOrder(book, {6, Side::Sell, 99.0, 3});  // crosses order 1, partially
    expect(trades6.size() == 1 && trades6[0].buy_order_id == 1 && trades6[0].quantity == 3,
           "order 6 partially fills against resting buy order 1 at 100");

    expect(book.bestBid() == 100.0, "order 1's remaining quantity (2) still rests at 100");
    expect(book.bestAsk() == 106.0, "order 4 is the only resting ask left, at 106");
}

int main() {
    vector test_list = {
        test_restingOrderNoCross,
        test_addThenCancel,
        test_crossingOrderFullyFills,
        test_partialFillRests,
        test_priceTimePriority,
        test_cancelOrder,
        test_manyOrdersMixedTransactions
    };
    for (auto test_num = 0; test_num < static_cast<int>(test_list.size()); test_num++) {
        cout << "test " << test_num << "\n";
        test_list[test_num]();
    }
    return 0;
}
