//generated by exodus "compile ../sys/timedate2"
//#ifndef EXODUSDLFUNC_TIMEDATE2_H
#define EXODUSDLFUNC_TIMEDATE2_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_timedate2;
class efb_timedate2 : private ExodusFunctorBase
{
public:

efb_timedate2(MvEnvironment& mv) : ExodusFunctorBase("timedate2", "exodusprogrambasecreatedelete_", mv) {}

efb_timedate2& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in localdate0="", in localtime0="")
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_timedate2.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_timedate2.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (localdate0,localtime0);

}

};
efb_timedate2 timedate2{mv};
//#endif