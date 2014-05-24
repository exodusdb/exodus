//#ifndef EXODUSDLFUNC_UPDTASKS_H
#define EXODUSDLFUNC_UPDTASKS_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_updtasks;

//a member function with the right arguments, returning a var or void
var updtasks(in mode, io oldtaskid, io task, io errmsg)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_updtasks.pmemberfunction_==NULL)
  efb_updtasks.init("updtasks","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,io,io,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_updtasks.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_updtasks.pmemberfunction_)))
  (mode,oldtaskid,task,errmsg);

}
//#endif