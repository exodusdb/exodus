//generated by exodus "compile secid"
//#ifndef EXODUSDLFUNC_SECID_H
#define EXODUSDLFUNC_SECID_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_secid;
class efb_secid : private ExodusFunctorBase
{
public:

efb_secid(MvEnvironment& mv) : ExodusFunctorBase("secid", "exodusprogrambasecreatedelete_", mv) {}

efb_secid& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
}

//a member function with the right arguments, returning a var or void
var operator() (in input, in max)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_secid.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_secid.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (input,max);

}

};
efb_secid secid{mv};
//#endif
