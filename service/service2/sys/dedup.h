//generated by exodus "compile ../sys/dedup"
//#ifndef EXODUSDLFUNC_DEDUP_H
#define EXODUSDLFUNC_DEDUP_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_dedup;
class efb_dedup : private ExodusFunctorBase
{
public:

efb_dedup(MvEnvironment& mv) : ExodusFunctorBase("dedup", "exodusprogrambasecreatedelete_", mv) {}

efb_dedup& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in mode, io list1, io list2, in seps)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,io,io,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_dedup.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_dedup.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (mode,list1,list2,seps);

}

var operator() (in mode, io list1, in list2, in seps) {
 var list2_io;
 if (list2.assigned()) list2_io=list2;
 return operator()(mode,list1,list2_io,seps);
}

var operator() (in mode, in list1, in list2, in seps) {
 var list2_io;
 if (list2.assigned()) list2_io=list2;
 var list1_io;
 if (list1.assigned()) list1_io=list1;
 return operator()(mode,list1_io,list2_io,seps);
}

};
efb_dedup dedup{mv};
//#endif
