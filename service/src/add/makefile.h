//generated by exodus "compile ../add/makefile"
//#ifndef EXODUSDLFUNC_MAKEFILE_H
#define EXODUSDLFUNC_MAKEFILE_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_makefile;
class efb_makefile : private ExodusFunctorBase
{
public:

efb_makefile(MvEnvironment& mv) : ExodusFunctorBase("makefile", "exodusprogrambasecreatedelete_", mv) {}

efb_makefile& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() ()
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)();

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_makefile.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_makefile.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  ();

}
};
efb_makefile makefile{mv};
//#endif
