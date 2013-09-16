#ifndef mugen_converter_state_controllers
#define mugen_converter_state_controllers

#include "tools.h"

#include <map>
#include <vector>

namespace Ast{
    class AttributeSimple;
}

namespace Mugen{
    
/*! State Parameter Map */
class StateParameterMap{
    public:
        StateParameterMap();
        StateParameterMap(const StateParameterMap &);
        virtual ~StateParameterMap();
        
        const StateParameterMap & operator=(const StateParameterMap &);
        
        void addTriggerall(Ast::AttributeSimple *);
        void addTrigger(int number, Ast::AttributeSimple *);
        void addVar(Ast::AttributeSimple *);
        void add(const std::string &, Ast::AttributeSimple *);
        
        std::vector<Ast::AttributeSimple *> * find(const std::string &);
        
        //! add to definition
        void addToDefinition(PythonDefinition &);
        
    private:
        std::vector<Ast::AttributeSimple *> triggerall;
        std::map< int, std::vector<Ast::AttributeSimple *> > triggers;
        std::vector<Ast::AttributeSimple *> vars;
        std::map<std::string, std::vector<Ast::AttributeSimple *> > parameters;
};
    
/*! State Controller store */
class StateControllerStore{
    public:
        StateControllerStore();
        virtual ~StateControllerStore();
        
        void newController();
        
        StateParameterMap & getCurrentController();
        
        void addToDefinition(PythonDefinition &);
        
    protected:
        std::vector<StateParameterMap> controllerParameters;
};
    
/*! State Controller Base Class */
class StateController{
    public:
        StateController(const StateParameterMap &);
        virtual ~StateController();
        
        virtual Content getContent() = 0;
        
    protected:    
        StateParameterMap parameters;
};
    
}

#endif