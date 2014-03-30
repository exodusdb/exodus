//#ifndef EXODUSDLFUNC_READBAKPARS_H
#define EXODUSDLFUNC_READBAKPARS_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_readbakpars;

//a member function with the right arguments, returning a var or void
var readbakpars(io bakpars, in process0=var())
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_readbakpars.pmemberfunction_==NULL)
  efb_readbakpars.init("readbakpars","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(io,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_readbakpars.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_readbakpars.pmemberfunction_)))
  (bakpars,process0);

}
//#endif
