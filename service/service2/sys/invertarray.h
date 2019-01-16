//generated by exodus "compile ../sys/invertarray"
//#ifndef EXODUSDLFUNC_INVERTARRAY_H
#define EXODUSDLFUNC_INVERTARRAY_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_invertarray;
class efb_invertarray : private ExodusFunctorBase
{
public:

efb_invertarray(MvEnvironment& mv) : ExodusFunctorBase("invertarray", "exodusprogrambasecreatedelete_", mv) {}

efb_invertarray& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in input, in force0=(0))
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
 //return CALLMEMBERFUNCTION(*(efb_invertarray.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_invertarray.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (input,force0);

}

};
efb_invertarray invertarray{mv};
//#endif
