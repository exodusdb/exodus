//generated by exodus "compile datetime"
//#ifndef EXODUSDLFUNC_DATETIME_H
#define EXODUSDLFUNC_DATETIME_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_datetime;
class efb_datetime : public ExodusFunctorBase
{
public:

efb_datetime(MvEnvironment& mv) : ExodusFunctorBase("datetime", "exodusprogrambasecreatedelete_", mv) {}

efb_datetime& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in type, in inx, in mode, out outx)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,in,out);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_datetime.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_datetime.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (type,inx,mode,outx);

}

var operator() () {
 var type_in;
 var inx_in;
 var mode_in;
 var outx_out;
 return operator()(type_in, inx_in, mode_in, outx_out);
}

var operator() (in type) {
 var inx_in;
 var mode_in;
 var outx_out;
 return operator()(type, inx_in, mode_in, outx_out);
}

var operator() (in type, in inx) {
 var mode_in;
 var outx_out;
 return operator()(type, inx, mode_in, outx_out);
}

var operator() (in type, in inx, in mode) {
 var outx_out;
 return operator()(type, inx, mode, outx_out);
}

};
efb_datetime datetime{mv};
//#endif