//generated by exodus "compile ../sys/log2"
//#ifndef EXODUSDLFUNC_LOG2_H
#define EXODUSDLFUNC_LOG2_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_log2;
class efb_log2 : private ExodusFunctorBase
{
public:

efb_log2(MvEnvironment& mv) : ExodusFunctorBase("log2", "exodusprogrambasecreatedelete_", mv) {}

efb_log2& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in msg0, io time0)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_log2.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_log2.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (msg0,time0);

}

var operator() (in msg0, in time0=var()) {
 var time0_io;
 if (time0.assigned()) time0_io=time0;
 return operator()(msg0,time0_io);
}

};
efb_log2 log2{mv};
//#endif
