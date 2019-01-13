//generated by exodus "compile ../sys/select2"
//#ifndef EXODUSDLFUNC_SELECT2_H
#define EXODUSDLFUNC_SELECT2_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_select2;
class efb_select2 : private ExodusFunctorBase
{
public:

efb_select2(MvEnvironment& mv) : ExodusFunctorBase("select2", "exodusprogrambasecreatedelete_", mv) {}

efb_select2& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
}

//a member function with the right arguments, returning a var or void
var operator() (in filenamex, in linkfilename2, in sortselect0, in dictids0, in options, io datax, io response, in limitfields0="", in limitchecks="", in limitvalues="", int maxnrecs=0)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,in,in,in,io,io,in,in,in,int);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_select2.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_select2.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (filenamex,linkfilename2,sortselect0,dictids0,options,datax,response,limitfields0,limitchecks,limitvalues,maxnrecs);

}

};
efb_select2 select2{mv};
//#endif
