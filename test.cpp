#include "regexengine.hpp"

int main() {
    RegexEngine engine("a*(bcd|ef)+");
    bool result = engine.match("bcd");
    result = engine.match("badabcd");
    result = engine.match("bcdkf");

    RegexEngine secengine("^a*(bcd|ef)+$");
    bool secresult = secengine.match("bcd");
    secresult = secengine.match("badabcd");    
}
