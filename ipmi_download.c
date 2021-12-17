#include <kernel.h>
#include <paf.h>
#include <ipmi.h>

namespace sce {

	class Download
	{
	public:

		class DownloadParam
		{
		public:

			SceInt32 unk_00; //0x1
			SceUChar8 unk_04[0x18];
			SceChar8 url[0x7FF];
			SceUChar8 unk_81C[0x204];
		};

		class HttpParam
		{
		public:

			SceInt32 paramType1; //0x1
			SceUInt32 paramVal1; //4000000
			SceInt32 paramType2; //0x2
			SceUInt32 paramVal2; //30000000
			SceInt32 paramType3; //0x0
			SceUInt32 paramVal3; //30000000
			SceChar8 url[0x7FF];
		};

		class AuthParam
		{
		public:

			SceUChar8 unk_00[0x441];
		};

		class MetadataInfo
		{
		public:

			SceUChar8 unk_00[0x38];
			SceUInt32 size;
			SceInt32 unk_3C;
			SceChar8 name[0xC1];
			SceChar8 mimeType[0x20];
			SceChar8 creationDateString[0x20];
			SceUChar8 unk_141[0x19F];
		};

		class ConnectionOpt
		{
		public:

			SceUChar8 unk_00[0x10];
			SceInt32 budgetType;
		};

		SceUID unk_00;
		SceUID unk_04;
		SceUID unk_08;
		IPMI::Client *client;
		ScePVoid clientMem;
		ScePVoid bufMem;
	};
}

int main()
{
	int ret = 0;
	int ret2 = 0;

	sce::Download dw;
	sce_paf_memset(&dw, 0, sizeof(sce::Download));

	IPMI::Client::Config conf;
	sce_paf_memset(&conf, 0, sizeof(IPMI::Client::Config));

	sce_paf_strncpy((char *)conf.name, "SceDownload", sizeof(conf.name));
	conf.msgPipeBudgetType = 0;
	conf.numREObjects = 1;
	conf.requestQueueSize = 0x1E00;
	conf.receptionQueueSize = 0x1E00;
	conf.numARObjects = 1;
	conf.requestPipeAddsize1 = 0xF00;
	conf.requestPipeAddsize2 = 0xF00;
	conf.numEventFlags = 1;
	conf.numMessages = 0;
	conf.pipeSmth = SCE_UID_INVALID_UID;

	SceUInt32 clMemSize = conf.estimateClientMemorySize();
	dw.clientMem = sce_paf_malloc(clMemSize);
	dw.bufMem = sce_paf_malloc(0x1000);

	IPMI::Client *client;
	ret = IPMI::Client::create(&client, &conf, &dw, dw.clientMem);
	sceClibPrintf("IPMI::Client::create: 0x%X\n", ret);

	dw.client = client;

	sce::Download::ConnectionOpt connOpt;
	sce_paf_memset(&connOpt, 0, sizeof(sce::Download::ConnectionOpt));
	connOpt.budgetType = conf.msgPipeBudgetType;

	ret = client->connect(&connOpt, sizeof(sce::Download::ConnectionOpt), &ret2);
	sceClibPrintf("client->connect: 0x%X\n", ret);
	sceClibPrintf("client->connect ret2: 0x%X\n", ret2);

	dw.unk_00 = SCE_UID_INVALID_UID;
	dw.unk_04 = SCE_UID_INVALID_UID;
	dw.unk_08 = SCE_UID_INVALID_UID;

	sce::Download::HttpParam hparam;
	sce::Download::AuthParam aparam;
	sce::Download::DownloadParam dparam;
	sce::Download::MetadataInfo minfo;
	sce_paf_memset(&hparam, 0, sizeof(sce::Download::HttpParam));
	sce_paf_memset(&aparam, 0, sizeof(sce::Download::AuthParam));
	sce_paf_memset(&dparam, 0, sizeof(sce::Download::DownloadParam));
	sce_paf_memset(&minfo, 0, sizeof(sce::Download::MetadataInfo));

	hparam.paramType1 = 1;
	hparam.paramVal1 = 4000000;
	hparam.paramType2 = 2;
	hparam.paramVal2 = 30000000;
	hparam.paramType2 = 0;
	hparam.paramVal2 = 30000000;
	sce_paf_strcpy((char *)hparam.url, "https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png");

	IPMI::DataInfo dtInfo;
	dtInfo.data1 = &hparam;
	dtInfo.data1Size = sizeof(sce::Download::HttpParam);
	dtInfo.data2 = &aparam;
	dtInfo.data2Size = sizeof(sce::Download::AuthParam);

	IPMI::BufferInfo bfInfo;
	bfInfo.data1 = &minfo;
	bfInfo.data1Size = sizeof(sce::Download::MetadataInfo);

	ret = client->invokeSyncMethod(0x1234000D, &dtInfo, 2, &ret2, &bfInfo, 1);
	sceClibPrintf("client->invokeSyncMethod: 0x%X\n", ret);
	sceClibPrintf("client->invokeSyncMethod ret2: 0x%X\n", ret2);

	sceClibPrintf("name: %s\n", minfo.name);
	sceClibPrintf("mime: %s\n", minfo.mimeType);
	sceClibPrintf("size: %u\n", minfo.size);

	dparam.unk_00 = 1;
	sce_paf_strcpy((char *)dparam.url, "https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png");

	dtInfo.data1 = &dparam;
	dtInfo.data1Size = sizeof(sce::Download::DownloadParam);
	dtInfo.data2 = &aparam;
	dtInfo.data2Size = sizeof(sce::Download::AuthParam);
	dtInfo.data3 = &minfo;
	dtInfo.data3Size = sizeof(sce::Download::MetadataInfo);

	SceInt32 dwRes = 0;
	bfInfo.data1 = &dwRes;
	bfInfo.data1Size = sizeof(SceInt32);

	ret = client->invokeSyncMethod(0x12340011, &dtInfo, 3, &ret2, &bfInfo, 1);
	sceClibPrintf("client->invokeSyncMethod: 0x%X\n", ret);
	sceClibPrintf("client->invokeSyncMethod ret2: 0x%X\n", ret2);
	sceClibPrintf("dwRes: 0x%X\n", dwRes);

	return 0;
}
