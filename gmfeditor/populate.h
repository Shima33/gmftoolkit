class Uni{
	public:
		void* object;
		int type;
		string label;	
};

char *StrToC(string str);
char *RGBToC(RGB color);
HTREEITEM Add();
HTREEITEM AddString(char* label, string* property);
HTREEITEM AddLabel(char* label, void* structure, int type);
HTREEITEM AddInt(char* label, int* property);
HTREEITEM AddRGB(char* label, RGB* property);
HTREEITEM AddFloat(char* label, float* property);
TVITEM HTREEITEMToTVITEM(HTREEITEM item);
void* getPointer(HTREEITEM item);
HTREEITEM ReadTextures(HTREEITEM parent, cGMFTextureList* list);
HTREEITEM ReadMaterials(HTREEITEM parent, cGMFMaterialList* list);
void ReadTM(cGMFTM* tm);
void ReadMesh(cGMFMesh* mesh);

#define TM 597
#define TMROW 598
