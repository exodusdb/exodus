//generated by exodus "compile usersubs"
//#ifndef EXODUSDLFUNC_USERSUBS_H
#define EXODUSDLFUNC_USERSUBS_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_usersubs;
class efb_usersubs : public ExodusFunctorBase
{
public:

efb_usersubs(MvEnvironment& mv) : ExodusFunctorBase("usersubs", "exodusprogrambasecreatedelete_", mv) {}

efb_usersubs& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in mode)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 //if (efb_getlang.pmemberfunction_==NULL)
 // efb_getlang.init("getlang","exodusprogrambasecreatedelete_",mv);
 if (this->pmemberfunction_==NULL)
  this->init();

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_usersubs.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_usersubs.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (mode);

}

var operator() () {
 var mode_in;
 return operator()(mode_in);
}

};
efb_usersubs usersubs{mv};
//#endif