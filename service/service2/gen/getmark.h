//#ifndef EXODUSDLFUNC_GETMARK_H
#define EXODUSDLFUNC_GETMARK_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_getmark;

//a member function with the right arguments, returning a var or void
var getmark(in mode, in html, io mark)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_getmark.pmemberfunction_==NULL)
  efb_getmark.init("getmark","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_getmark.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_getmark.pmemberfunction_)))
  (mode,html,mark);

}
//#endif
