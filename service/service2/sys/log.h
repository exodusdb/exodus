//#ifndef EXODUSDLFUNC_LOG_H
#define EXODUSDLFUNC_LOG_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_log;

//a member function with the right arguments, returning a var or void
var log(in programname0, in text0)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_log.pmemberfunction_==NULL)
  efb_log.init("log","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_log.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_log.pmemberfunction_)))
  (programname0,text0);

}
//#endif
