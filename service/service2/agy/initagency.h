//generated by exodus "compile ../agy/initagency"
//#ifndef EXODUSDLFUNC_INITAGENCY_H
#define EXODUSDLFUNC_INITAGENCY_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_initagency;
class efb_initagency : private ExodusFunctorBase
{
public:

efb_initagency(MvEnvironment& mv) : ExodusFunctorBase("initagency", "exodusprogrambasecreatedelete_", mv) {}

efb_initagency& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
}

//a member function with the right arguments, returning a var or void
var operator() ()
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)();

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_initagency.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_initagency.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  ();

}

};
efb_initagency initagency{mv};
//#endif
