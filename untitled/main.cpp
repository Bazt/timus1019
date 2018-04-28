#include <iostream>
#include <list>

using namespace std;

struct segment {
    int begin, end;

    segment(int begin = 0, int end = 0) : begin(begin), end(end) {}

    bool isProperSupersetOf(segment x) {
        return begin < x.begin && end > x.end;
    }

    bool intersectsWith(segment x) {
        int maxbegin = begin > x.begin ? begin : x.begin;
        int minend = end < x.end ? end : x.end;
        return maxbegin <= minend;
    }

    bool hasSharedPart(segment x) {
        return intersectsWith(x) && x.begin != end && x.end != begin;
    }

    bool equals(segment x) {
        return x.end == end && x.begin == begin;
    }

    segment unite(segment x) {
        segment result;
        result.begin = x.begin < begin ? x.begin : begin;
        result.end = x.end > end ? x.end : end;

        return result;

    }

    bool isEmpty() {
        return begin == end;
    }

    int length() {
        return end - begin;
    }
};

void processSegment(segment &current, list<segment> &same, list<segment> &opposite) {
    // ======================================================
    //       update opposite list

    for (auto opposite_it = opposite.begin(); opposite_it != opposite.end(); opposite_it++) {

        if (opposite_it->equals(current) || current.isProperSupersetOf(*opposite_it)) {
            opposite_it = opposite.erase(opposite_it);
            continue;
        }

        if (opposite_it->isProperSupersetOf(current)) {
            segment first(opposite_it->begin, current.begin);
            opposite_it->begin = current.end;
            opposite.insert(opposite_it, first);
            continue;
        }
    }

    for (auto white_it = opposite.begin(); white_it != opposite.end(); white_it++) {
        if (white_it->hasSharedPart(current)) {
            white_it->begin = current.end > white_it->begin ? current.end : white_it->begin;
            white_it->end = current.begin > white_it->end ? white_it->end : current.begin;
        }
    }

    //===============================================
    //           update same list
    for (auto it = same.begin(); it != same.end(); it++) {
        if (it->isProperSupersetOf(current)) {
            continue;
        }
        if (current.isProperSupersetOf(*it)) {
            it = same.erase(it);
        }
    }

    for (auto it = same.begin(); it != same.end(); it++) {
        if (it->intersectsWith(current)) {
            current = current.unite(*it);
            it = same.erase(it);
        }
    };

    for (auto it = same.begin(); it != same.end(); it++) {
        if (it->begin > current.end) {
            same.insert(it, current);
        }
    }

    if (same.empty()) {
        same.push_back(current);
    }

}


int main() {

    segment current;
    char color;
    int begin, end;
    int n;
    cin >> n;
    const int r = n;
    list<segment> black{};
    list<segment> white{segment(0, 1000000000)};
    for (int i = 0; i < r; i++) {
        cin >> begin >> end >> color;
        current.begin = begin;
        current.end = end;
        if (color == 'b') {
            processSegment(current, black, white);

        } else {
            processSegment(current, white, black);
        }
    }

    segment s;
    for (auto x : white) {
        if (x.length() > s.length()) {
            s = x;
        }
    }

    cout << s.begin << " " << s.end;


    return 0;
}