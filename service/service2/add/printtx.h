//generated by exodus "compile ../add/printtx"
//#ifndef EXODUSDLFUNC_PRINTTX_H
#define EXODUSDLFUNC_PRINTTX_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_printtx;
class efb_printtx : private ExodusFunctorBase
{
public:

efb_printtx(MvEnvironment& mv) : ExodusFunctorBase("printtx", "exodusprogrambasecreatedelete_", mv) {}

efb_printtx& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (io tx, in mode="", in modevalue="")
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(io,in,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_printtx.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_printtx.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (tx,mode,modevalue);

}

var operator() ( in tx=var(), in mode="", in modevalue="") {
 var tx_io;
 if (tx.assigned()) tx_io=tx;
 return operator()(tx_io,mode,modevalue);
}

};
efb_printtx printtx{mv};
//#endif
