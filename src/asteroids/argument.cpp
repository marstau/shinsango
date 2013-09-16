#include "argument.h"
#include "util/debug.h"
#include "game.h"

using std::vector;
using std::string;
using std::endl;

namespace Asteroids{

class AsteroidsArgument: public Argument {
public:
    vector<string> keywords() const {
        vector<string> out;
        out.push_back("asteroids");
        return out;
    }

    string description() const {
        return " : Run asteroids";
    }

    class Run: public ArgumentAction {
    public:
        virtual void act(){
            run();
        }
    };

    vector<string>::iterator parse(vector<string>::iterator current, vector<string>::iterator end, ActionRefs & actions){
        actions.push_back(::Util::ReferenceCount<ArgumentAction>(new Run()));
        return current;
    }
};

std::vector< ::Util::ReferenceCount<Argument> > arguments(){
    vector< ::Util::ReferenceCount<Argument> > all;
    all.push_back(::Util::ReferenceCount<Argument>(new AsteroidsArgument()));
    return all;
}

}
