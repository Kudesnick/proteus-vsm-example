#include "stdafx.h"
#include "vsm.hpp"

#define VSMModel_key 0x00000000

class vsmDsim : public IDSIMMODEL
{
public:
	virtual INT isdigital(CHAR* pinname) override { return 1; }
	virtual VOID setup(IINSTANCE* inst, IDSIMCKT* dsim) override { this->inst = inst; this->ckt = dsim; }
	virtual VOID runctrl(RUNMODES mode) override {}
	virtual VOID actuate(REALTIME time, ACTIVESTATE newstate) override {}
	virtual BOOL indicate(REALTIME time, ACTIVEDATA* data) override { return TRUE; }
	virtual VOID simulate(ABSTIME time, DSIMMODES mode) override {}
	virtual VOID callback(ABSTIME time, EVENTID eventid) override {}
private:
	IINSTANCE* inst = NULL;
	IDSIMCKT* ckt = NULL;
};

class vsmActive : public IACTIVEMODEL {
public:
    virtual VOID initialize(ICOMPONENT* cpt) override { this->cpt = cpt; }
    virtual ISPICEMODEL* getspicemodel(CHAR* p) override { return NULL; }
    virtual IDSIMMODEL* getdsimmodel(CHAR* p) override { return new vsmDsim; }
    virtual VOID plot(ACTIVESTATE state) override {}
    virtual VOID animate(INT element, ACTIVEDATA* newstate) override {}
    virtual BOOL actuate(WORD key, INT x, INT y, DWORD flags) override { return FALSE; }
private:
	ICOMPONENT* cpt = NULL;
};

/* Вызов конструктора цифровой модели. Вызывается, если Proteus определил компонент как чисто цифровой */
extern "C" {
	IDSIMMODEL __declspec (dllexport)* createdsimmodel(CHAR* device, ILICENCESERVER* ils) {
		return new vsmDsim;
	}
}

extern "C" VOID __declspec (dllexport) deletedsimmodel(IDSIMMODEL* model) {
	delete (IDSIMMODEL*)model;
}

/* Вызов конструктора активной модели. Вызывается, если Proteus определил что компонент будет отрисовываться
средствами dll. Для этого прои добавлении компонента в пользовательскую библиотеку в его Script Block должен быть
прописан параметр ACTIVE например так: {ACTIVE=VSMACTIVE,0,DLL}. В этом случае createdsimmodel не будет вызываться,
а конструктор цифровой модели будет вызван из метода getdsimmodel возвращаемого объекта */
extern "C" {
	__declspec(dllexport) IACTIVEMODEL* createactivemodel(CHAR* device, ILICENCESERVER* ils) {
		return new vsmActive;
	}
}

extern "C" {
	VOID  __declspec(dllexport) deleteactivemodel(IACTIVEMODEL* model) {
		delete (IACTIVEMODEL*)model;
	}
}
