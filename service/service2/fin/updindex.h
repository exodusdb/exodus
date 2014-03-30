//#ifndef EXODUSDLFUNC_UPDINDEX_H
#define EXODUSDLFUNC_UPDINDEX_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_updindex;

//a member function with the right arguments, returning a var or void
var updindex(in postingsindex, in indexkey, in daterefline, in deleting)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_updindex.pmemberfunction_==NULL)
  efb_updindex.init("updindex","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,in,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_updindex.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_updindex.pmemberfunction_)))
  (postingsindex,indexkey,daterefline,deleting);

}
//#endif
