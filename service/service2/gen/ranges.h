#ifndef EXODUSDLFUNC_RANGES_H
#define EXODUSDLFUNC_RANGES_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_ranges;

//a member function with the right arguments, returning a var
var ranges(io ranges0, in basekey0, in listid="")
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_ranges.pmemberfunction_==NULL)
  efb_ranges.init("ranges","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(io,in,in);

 //call the shared library object main function with the right args, returning a var
 return CALLMEMBERFUNCTION(*(efb_ranges.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_ranges.pmemberfunction_)))
  (ranges0,basekey0,listid);

}
#endif
