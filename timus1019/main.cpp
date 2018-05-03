#include <iostream>
#include <list>

using namespace std;

struct segment {
    int begin, end;

    static const int maxValue = 1000000000;

    segment(int begin = 0, int end = 0) : begin(begin), end(end) {}

    bool isProperSupersetOf(segment &x) {
        return begin < x.begin && end > x.end;
    }

    bool isSupersetOf(segment &x) {
        return begin <= x.begin && end >= x.end;
    }

    bool intersectsWith(segment &x) {
        int maxbegin = begin > x.begin ? begin : x.begin;
        int minend = end < x.end ? end : x.end;
        return maxbegin <= minend;
    }

    bool equals(segment &x) {
        return x.end == end && x.begin == begin;
    }

    segment unite(segment &x) {
        segment result;
        result.begin = x.begin < begin ? x.begin : begin;
        result.end = x.end > end ? x.end : end;

        return result;

    }

    bool isEmpty() {
        return begin == end;
    }

    bool locatedLeftFrom(segment &x) {
        return x.begin > end;
    }

    int length() {
        return end - begin;
    }
};

void processSegment(segment &current, list<segment> &same, list<segment> &opposite) {
    // ======================================================
    //       update opposite list

    list<list<segment>::iterator> toBeRemoved;
    for (auto opposite_it = opposite.begin(); opposite_it != opposite.end(); opposite_it++) {
        if (current.isSupersetOf(*opposite_it)) {
            toBeRemoved.push_back(opposite_it);

        }
    }
    for (auto it : toBeRemoved) {
        opposite.erase(it);
    }


    for (auto opposite_it = opposite.begin(); opposite_it != opposite.end(); opposite_it++) {

        segment op = *opposite_it;
        if (current.end <= opposite_it->begin || opposite_it -> end <= current.begin) {
            continue;
        }

        if (current.isSupersetOf(*opposite_it)) {
            opposite_it = opposite.erase(opposite_it);
            if (opposite_it != opposite.begin()) {
                opposite_it--;
            }
            op = *opposite_it;
        } else if (opposite_it->isProperSupersetOf(current)) {
            segment first(opposite_it->begin, current.begin);
            opposite_it->begin = current.end;
            opposite.insert(opposite_it, first);
            break;
        }
    }
    for (auto opposite_it = opposite.begin(); opposite_it != opposite.end(); opposite_it++) {
        if (current.end <= opposite_it->begin || opposite_it -> end <= current.begin) {
            continue;
        }
        if (current.intersectsWith(*opposite_it) ) {
            if (current.begin < opposite_it->begin)
            {
                opposite_it->begin = current.end;
            } else {
                opposite_it->end = current.begin;
            }
        }
    }




    //===============================================
    //           update same list

    if (same.empty()) {
        same.push_back(current);
    } else {
        for (auto it = same.begin();;) {
            if (it == same.end() || current.locatedLeftFrom(*it)) {
                same.insert(it, current);
                break;
            }
            if (current.intersectsWith(*it)) {
                current = current.unite(*it);
                it = same.erase(it);
                continue;
            }
            it++;

        }
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
    list<segment> white{segment(0, segment::maxValue)};
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
