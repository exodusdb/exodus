//generated by exodus "compile ../sys/date"
//#ifndef EXODUSDLFUNC_DATE_H
#define EXODUSDLFUNC_DATE_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_date;
class efb_date : private ExodusFunctorBase
{
public:

efb_date(MvEnvironment& mv) : ExodusFunctorBase("date", "exodusprogrambasecreatedelete_", mv) {}

efb_date& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in type, io in0, io mode, out output, in glang="")
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,io,io,out,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_date.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_date.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (type,in0,mode,output,glang);

}

};
efb_date date{mv};
//#endif