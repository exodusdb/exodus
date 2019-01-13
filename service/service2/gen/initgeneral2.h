//generated by exodus "compile initgeneral2"
//#ifndef EXODUSDLFUNC_INITGENERAL2_H
#define EXODUSDLFUNC_INITGENERAL2_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_initgeneral2;
class efb_initgeneral2 : private ExodusFunctorBase
{
public:

efb_initgeneral2(MvEnvironment& mv) : ExodusFunctorBase("initgeneral2", "exodusprogrambasecreatedelete_", mv) {}

efb_initgeneral2& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
}

//a member function with the right arguments, returning a var or void
var operator() (in mode, io logtime)
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
 //return CALLMEMBERFUNCTION(*(efb_initgeneral2.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_initgeneral2.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (mode,logtime);

}

};
efb_initgeneral2 initgeneral2{mv};
//#endif
