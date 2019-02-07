//generated by exodus "compile ../gen/daterangetext"
//#ifndef EXODUSDLFUNC_DATERANGETEXT_H
#define EXODUSDLFUNC_DATERANGETEXT_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_daterangetext;
class efb_daterangetext : private ExodusFunctorBase
{
public:

efb_daterangetext(MvEnvironment& mv) : ExodusFunctorBase("daterangetext", "exodusprogrambasecreatedelete_", mv) {}

efb_daterangetext& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in d1, in d2, out result, io lang)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,out,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_daterangetext.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_daterangetext.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (d1,d2,result,lang);

}

var operator() (in d1, in d2, out result, in lang=var()) {
 var lang_io;
 if (lang.assigned()) lang_io=lang;
 return operator()(d1,d2,result,lang_io);
}

var operator() (in d1, in d2, in result=var(), in lang=var()) {
 var lang_io;
 if (lang.assigned()) lang_io=lang;
 var result_out;
 if (result.assigned()) result_out=result;
 return operator()(d1,d2,result_out,lang_io);
}

};
efb_daterangetext daterangetext{mv};
//#endif
