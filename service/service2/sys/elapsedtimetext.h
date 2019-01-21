//generated by exodus "compile ../sys/elapsedtimetext"
//#ifndef EXODUSDLFUNC_ELAPSEDTIMETEXT_H
#define EXODUSDLFUNC_ELAPSEDTIMETEXT_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_elapsedtimetext;
class efb_elapsedtimetext : private ExodusFunctorBase
{
public:

efb_elapsedtimetext(MvEnvironment& mv) : ExodusFunctorBase("elapsedtimetext", "exodusprogrambasecreatedelete_", mv) {}

efb_elapsedtimetext& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in fromdate, in fromtime)
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
 //return CALLMEMBERFUNCTION(*(efb_elapsedtimetext.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_elapsedtimetext.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (fromdate,fromtime);

}

};
efb_elapsedtimetext elapsedtimetext{mv};
//#endif