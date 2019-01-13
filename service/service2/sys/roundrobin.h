//generated by exodus "compile ../sys/roundrobin"
//#ifndef EXODUSDLFUNC_ROUNDROBIN_H
#define EXODUSDLFUNC_ROUNDROBIN_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_roundrobin;
class efb_roundrobin : private ExodusFunctorBase
{
public:

efb_roundrobin(MvEnvironment& mv) : ExodusFunctorBase("roundrobin", "exodusprogrambasecreatedelete_", mv) {}

efb_roundrobin& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
}

//a member function with the right arguments, returning a var or void
var operator() (in mode, in params0, io msg, io result)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,io,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_roundrobin.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_roundrobin.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (mode,params0,msg,result);

}

};
efb_roundrobin roundrobin{mv};
//#endif
