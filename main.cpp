#include "Datamosh.h"


int main() {

	using namespace  IDataMosh;
	//LIVE OR VIDEO -+-+-+-+- video is beta
	Datamosh::instanse().SetEngine(Datamosh::STATUS::LIVE);
	
	Datamosh::instanse().SetSettings(25,50);

	Datamosh::instanse().Start();
	

	return 0;
}