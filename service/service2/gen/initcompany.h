//generated by exodus "compile ../gen/initcompany"
//#ifndef EXODUSDLFUNC_INITCOMPANY_H
#define EXODUSDLFUNC_INITCOMPANY_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_initcompany;
class efb_initcompany : private ExodusFunctorBase
{
public:

efb_initcompany(MvEnvironment& mv) : ExodusFunctorBase("initcompany", "exodusprogrambasecreatedelete_", mv) {}

efb_initcompany& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in nextcompanycode)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_initcompany.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_initcompany.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (nextcompanycode);

}
};
efb_initcompany initcompany{mv};
//#endif
