#include "regexengine.hpp"

int main() {
    try {
        RegexEngine engine("[a-z]*(bcd|ef)+");
        bool result = engine.match("bcd");
        result = engine.match("abcdbcd");
        result = engine.match("bcdkf");
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
}
