#include "ModelBuildCmd.h"
#include "Common/Operator.h"
#include "Common/ModelBuildArgs.h"
#include "Core/Core.h"
#include "Common/ServerListening.h"
#include "json.hpp"
using namespace EPGIS_3D;

int main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "");
	LogIns.initLog();

	// 公用参数
	ModelBuildCmd command = ModelBuildCmd(argv[0], "0.0.1");
	std::string strf = Tr("Specify file path (file information for the model, which can be a file or folder)");
	command.option("-f", "--file-path <file or dir>", strf.c_str(), ModelBuildCmd::SetFilePath);
	std::string strdn = Tr("Specify dataset name ");
	command.option("-dn", "--dataset-name <dataset name>", strdn.c_str(), ModelBuildCmd::SetDatasetName);
	std::string stro = Tr("Specify the output directory for the tileset");
	command.option("-o", "--output-dir <dir>", stro.c_str(), ModelBuildCmd::SetOutputDir);
	std::string strt = Tr("Specify file extension (file format)");
	command.option("-t", "--file-ext <ext>", strt.c_str(), ModelBuildCmd::SetFileExt);
	std::string strdb = Tr("Specifies the database or file system");
	command.option("-db", "--database <database>", strdb.c_str(), ModelBuildCmd::SetStorageType);
	std::string strdbconn = Tr("Specifies the database connection information");
	command.option("-dbconn", "--dbconn <dbconn>", strdbconn.c_str(), ModelBuildCmd::SetDbConnectSetting);
	std::string strgz = Tr("Specifies the switch to use GZip compression");
	command.option("-gz", "--GZip <0 or 1>", strgz.c_str(), ModelBuildCmd::SetGZipCompression);
	std::string strepgis = Tr("Specifies the switch to use epgis convert");
	command.option("-epgis", "--Convert-epgiscoords<0 or 1>", strepgis.c_str(), ModelBuildCmd::SetEpGisConvert);
	std::string strepgishost = Tr("Specifies the switch to use epgis convert");
	command.option("-epgishost", "--epgiscoords-host<host>", strepgishost.c_str(), ModelBuildCmd::SetEpGisConverthost);
	std::string strepgiskey = Tr("Specifies the switch to use epgis convert");
	command.option("-epgiskey", "--epgiscoords-key<key>", strepgiskey.c_str(), ModelBuildCmd::SetEpGisConvertKey);
	std::string strepgissecret = Tr("Specifies the switch to use epgis convert");
	command.option("-epgissecret", "--epgiscoords-secret<secret>", strepgissecret.c_str(), ModelBuildCmd::SetEpGisConvertSecret);
	std::string strul = Tr("Specifies the json file for user lod strategy");
	command.option("-ul", "--UserLod <file>", strul.c_str(), ModelBuildCmd::SetUserLodSetting);
	std::string strs = Tr("Specifies the start level at which the file is produced");
	command.option("-s", "--StartLevel <level>", strs.c_str(), ModelBuildCmd::SetStartLevel);
	std::string stre = Tr("Specifies the end level at which the file is produced");
	command.option("-e", "--EndLevel <level>", stre.c_str(), ModelBuildCmd::SetEndLevel);
	std::string strul_start = Tr("User Lod parameter for start level");
	command.option("-ul_start", "--UserLodStart <level>", strul_start.c_str(), ModelBuildCmd::SetUserLod_Start);
	std::string strul_end = Tr("User Lod parameter for end level");
	command.option("-ul_end", "--UserLodEnd <level>", strul_end.c_str(), ModelBuildCmd::SetUserLod_End);
	std::string strul_sply = Tr("User Lod parameter for simplify level");
	command.option("-ul_sply", "--UserLodSimplify <level>", strul_sply.c_str(), ModelBuildCmd::SetUserLod_SimplifyLevel);
	std::string strcf = Tr("Specifies the json file for global config");
	command.option("-cf", "--UserConfig <file>", strcf.c_str(), ModelBuildCmd::SetConfigFile);
	std::string strsv = Tr("used when server use tool");
	command.option("-sv", "--server", strsv.c_str(), ModelBuildCmd::SetCallByServer);
	std::string strlogpath = Tr("Specifies the path to the log");
	command.option("-logpath", "--log-path <projInfo>", strlogpath.c_str(), ModelBuildCmd::SetLogPath);
	
	// 白模
	std::string strhf = Tr("Specifies height field of shape file");
	command.option("-hf", "--Height-Field<height>", strhf.c_str(), ModelBuildCmd::SetHeightField);
	std::string strff = Tr("Specifies floor field of shape file");
	command.option("-ff", "--Floor-Field<field>", strff.c_str(), ModelBuildCmd::SetFloorField);
	std::string strfhf = Tr("Specifies floor height field of shape file");
	command.option("-fhf", "--FloorHeight-Field<field>", strfhf.c_str(), ModelBuildCmd::SetFloorHeightField);
	std::string strws = Tr("Specifies configure of white model");
	command.option("-ws", "--WhiteModel-Setting<path>", strws.c_str(), ModelBuildCmd::SetWhiteModelSetting);
	
	//  白模支持  -thq
	std::string strdapc = Tr("Specifies the switch to display as point cloud");
	command.option("-dapc", "--DisplayAsPointClound <0 or 1>", strdapc.c_str(), ModelBuildCmd::SetDisplayAsPointCloud);
	std::string strms = Tr("Specifies the json file as record to modify exist tileset");
	command.option("-ms", "--ModifySource <file>", strms.c_str(), ModelBuildCmd::SetModifySource);
	std::string strmt = Tr("Specifies the tileset path to modify");
	command.option("-mt", "--ModifyTarget <dir>", strmt.c_str(), ModelBuildCmd::SetModifyTarget);
	std::string strbk = Tr("Specifies the temp path to perform benchmark test");
	command.option("-bk", "--BenchMark <path>", strbk.c_str(), ModelBuildCmd::SetBenchMarkTempOutputDir);
	std::string strdds = Tr("Specifies the switch to use dds compress texture");
	command.option("-dds", "--DDS <0 or 1>", strdds.c_str(), ModelBuildCmd::SetUseDDSCompressTexture);
	std::string strdraco = Tr("Specifies the level to use draco to compress model,default=7");
	command.option("-draco", "--Draco-Level <level>", strdraco.c_str(), ModelBuildCmd::SetUseDracoCompression);
	std::string strqp = Tr("Quantization bits for the position attribute,default=16");
	command.option("-qp", "--Quantization-Position <bits>", strqp.c_str(), ModelBuildCmd::SetDracoQPosition);
	std::string strqn = Tr("Quantization bits for the normal attribute,default=16");
	command.option("-qn", "--Quantization-Normal <bits>", strqn.c_str(), ModelBuildCmd::SetDracoQNormal);
	std::string strqt = Tr("Quantization bits for the texture coord attribute,default=16");
	command.option("-qt", "--Quantization-TexCoord <bits>", strqt.c_str(), ModelBuildCmd::SetDracoQTexCoord);
	std::string strqg = Tr("Quantization bits for the generic attribute,default=16");
	command.option("-qg", "--Quantization-Generic <bits>", strqg.c_str(), ModelBuildCmd::SetDracoQGeneric);
	std::string strextra = Tr("Extract Models (.glb)");
	command.option("-extra", "--Extract-Models", strextra.c_str(), ModelBuildCmd::SetExtractModels);
	std::string strbtxt = Tr("Specifies height field of shape file");
	command.option("-btxt", "--Building-Texture<json>", strbtxt.c_str(), ModelBuildCmd::SetBuildingTexture);
	std::string strexportThirdParty = Tr("Specifies to export for third party rendering engine or not");
	command.option("-exportThirdParty", "--export-third-party<0 or 1>", strexportThirdParty.c_str(), ModelBuildCmd::SetExportForThirdPartyRenderingEngine);
	std::string strcommon = Tr("Specifies to export GIM Common Model");
	command.option("-common", "--GIM-Common<0 or 1>", strcommon.c_str(), ModelBuildCmd::SetGIMCommon);
	std::string strglb = Tr("Specifies to export GLB Model");
	command.option("-glb", "--Out-GLB<0 or 1>", strglb.c_str(), ModelBuildCmd::SetOutGLB);

	std::string strgenConstructAnimation = Tr("Specifies to generate the construct animation for tileset or not");
	command.option("-genConstructAnimation", "--generate-construct-animation<0 or 1>", strgenConstructAnimation.c_str(), ModelBuildCmd::SetGenerateConstructAnimation);
	std::string strconstructAnimationMaxDays = Tr("Specifies the max days of the construct animation");
	command.option("-constructAnimationMaxDays", "--construct-animation-max-days<integer>", strconstructAnimationMaxDays.c_str(), ModelBuildCmd::SetConstructAnimationMaxDays);
	std::string strconstructAnimationType = Tr("Specifies the animation type to perform");
	command.option("-constructAnimationType", "--construct-animation-type<typename>", strconstructAnimationType.c_str(), ModelBuildCmd::SetConstructAnimationType);
	std::string strconstructAnimationStorageType = Tr("Specifies the animation storage type");
	command.option("-constructAnimationStorageType", "--construct-animation-storage-type<typename:VERTEX or JSON>", strconstructAnimationStorageType.c_str(), ModelBuildCmd::SetConstructAnimationStorageType);
	std::string strdd = Tr("Specify delete dir");
	command.option("-dd", "--delete-dir <0 or 1>", strdd.c_str(), ModelBuildCmd::SetDeleteDir);
	std::string strdebug = Tr("Write debug info to log file");
	command.option("-debug", "--write-debug-to-log-file", strdebug.c_str(), ModelBuildCmd::SetWriteDebugToLogFile);
	
	// Parse and check the arguments
	command.parse(argc, argv);
	command.check(argc);

	int successCode = 0;
	int errorCode = 1;
	bool result = true;
	ModelBuildArgs buildArgs = command.Get_ModelBuildArgs();
	
	std::shared_ptr<EPGIS_3D::Core> corePtr = std::make_shared<EPGIS_3D::Core>(buildArgs, JniProgress);
	
	float timeCost = 0.0f;
	result = corePtr->Build(timeCost);
	if (result) {
		corePtr->GetLogger().info("All Finished! Cost Time = {0} s", timeCost);
		return Exit(successCode);
	}
	else {
		if (buildArgs.mIsCallByServer && LogIns.GetLastErrorMsg().size()>0)
			LogIns.OutToStdErr("epgis-error-message:", LogIns.GetLastErrorMsg().back());
		return Exit(errorCode);
	}
	return Exit(successCode);
}


bool Core::Build(float & timeCost)
{
	bool result = true;

	time_t startTime, endTime;
	time(&startTime);
	
	#ifndef _DEBUG
	if (isCheckLicense)
	{
		result = CheckAllLicense();
		if (!result) {
			return false;
		}
	}
	#endif
	// Core初始化后，检测参数是否正确。
	if (!modelBuildArgs.CheckParameters())
		return false;

	if (!CheckCustomEpgisMapConfig(modelBuildArgs.mEpGisConvertFlag, modelBuildArgs.mEpGisConvertHost, modelBuildArgs.mEpGisConvertKey, modelBuildArgs.mEpGisConvertSecret))
		return false;
		
		// 检查文件路径与文件格式
	filepath = modelBuildArgs.mFilePath;
	filepath = StringUtil::ReplaceAllString(filepath, "\\", "/");

	// 设置源路径
	instanceLodDescPtr->SetSourcePathName(filepath);
	if (instanceLodDescPtr->Load()) {
		LogIns.debug("InstanceLodDesc.csv is found !!!");
	}
	instanceWaveDescPtr->SetSourcePathName(filepath);
	if (instanceWaveDescPtr->Load()) {
		LogIns.debug("InstanceWaveDesc.csv is found !!!");
	}


	format = modelBuildArgs.mFileExt;

	if (!SetOutputDir())
	{
		return false;
	}
	
	outDirectory = StringUtil::ReplaceAllString(outDirectory, "\\", "/");

	// 获得生产的经纬度与偏移坐标
	local_x = modelBuildArgs.mLocationX;
	local_y = modelBuildArgs.mLocationY;
	local_z = modelBuildArgs.mLocationZ;

	if (modelBuildArgs.mEpGisConvertFlag == 1 && epGisTransformationPtr)
	{
		if ((!Math::IsFloatEqual(local_x, 0.0) && !Math::IsFloatEqual(local_x, 0.0)) && !epGisTransformationPtr->ConvertEpGis(local_x, local_y))
		{
			LogIns.error("epgis transform failed!");
			return false;
		}
	}
	
	offsetX = modelBuildArgs.mTransformX;
	offsetY = modelBuildArgs.mTransformY;
	offsetZ = modelBuildArgs.mTransformZ;

	//根据是否压缩参数、存储方式设置数据集
	if (!SetModelDataSetPtr())
	{
		DeleteResourceFromStop();
		return false;
	}
	
	result = BuildShp();
	if (!result) {
		DeleteResourceFromStop();
		return false;
	}
	
	FinializeCoreObjects();

	if (!DeleteResourceFromStop())
	{
		return false;
	}
	if (modeDataSetPtr)
		modeDataSetPtr->AllFinished();
	time(&endTime);
	float time = difftime(endTime, startTime);
	LogIns.info("Total Time : {0}s", time);
	timeCost = time;

	return true;
}

#include <Common/Gzip.h>
#include <Common/stdafx.h>
#include <Common/StringUtil.h>
#include <Common/FileSystemUtil.h>
#include <Common/MathUtil.h>
#include <Common/Image.h>
#include <Common/StringConverter.h>
#include <Core/Core.h>
#include "json.hpp"
#include "Common/Log.h"
#include "Common/Extern.h"
#include "Common/Config.h"
#include "Common/Gzip.h"
#include "Common/Image.h"
#include "Common/Operator.h"
#include "License/License.h"
#include "RegistrationInfo/RegistrationInfo.h"
#include "EncryptionInfo/EncryptionInfo.h"
#include "Model/Model.h"
#include "Model/Shp2Model.h"
#include "Model/ModelProduceProgress.h"
#include "Model/DummyPointFetchManager.h"
#include "DataSet/UniversalModelDataSet.h"
#include "DataSet/TileModelDataSet.h"
#include "DataSet/DbTileModelDataSet.h"
#include "DataSet/LodStrategy.h"
#include "ModelLibrary/ModelLibrary.h"
#include "ModelLibrary/TransLineDataSerializer.h"
#include "ModelModify/FileDataEditHandler.h"
#include "ModelModify/DbDataEditHandler.h"
#include "GIM/GIMDataSet.h"
#include "Osgb/OsgbModelDataSet.h"
#include "DB/DbConnFactory.h"
#include "DB/PostgreSqlDB.h"
#include "Terrain/TBTool.h"
#include "Terrain/TerrainDataBuilder.h"
#include "Terrain/TerrainDataSet.h"
#include "Model/Shp2Model.h"
#include <DataSet/FileModelDataSet.h>
#include <DataSet/ExportModelDataSet.h>
#include <DataSet/GLTFModelDataSet.h>
#include <Terrain/TileMaker.h>
#include "Terrain/TerrainTileSetMerge.h"
#include "Terrain/TerrainTileSetUpdate.h"
#include "TileSchema/TileSchema.h"
#include "TileSchema/QuadTileSchema.h"
#include "Common/ThreadPool.h"
#include "ModelLibrary/TransmissionLineModelBuilder.h"
#include "TransLine/TLJsonModelBuilder.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace nlohmann;

namespace EPGIS_3D {
	bool Core::BuildShp()
	{
		if (!CheckCustomEpgisMapConfig(modelBuildArgs.mEpGisConvertFlag, modelBuildArgs.mEpGisConvertHost, modelBuildArgs.mEpGisConvertKey, modelBuildArgs.mEpGisConvertSecret))
			return false;
		Shp2ModelParam param;
		param.StartLevel = modelBuildArgs.mStartLevel;
		param.EndLevel = modelBuildArgs.mEndLevel;
		if (modelBuildArgs.mStorageType == 0)
			param.OutputFilePath = FileSystem::CombinePath(modelBuildArgs.mOutputDir, modelBuildArgs.mDatasetName);

		param.PtrModelDataSet = modeDataSetPtr;
		param.HeightField = modelBuildArgs.mHeightField;
		std::string dirPath = modelBuildArgs.mFilePath;
		//std::string dirPath = StringConverter::ANSIToUTF8(buildArgs.mFilePath);
		if (FileSystem::FileExists(dirPath) && FileSystem::IsFileOrDir(dirPath) == -1)
		{
			param.ShapeFilePath = dirPath;
			param.SettingFilePath = modelBuildArgs.mWhiteModelSetting;
		}
		else if (FileSystem::DirExists(dirPath))
		{
			std::vector<std::string> shpFiles;
			FileSystem::GetFiles(dirPath, shpFiles, "shp", true);
			std::vector<std::string> jsonFiles;
			FileSystem::GetFiles(dirPath, jsonFiles, "json", true);
			if (shpFiles.empty())
				return false;
			param.ShapeFilePath = shpFiles[0];
			if (!jsonFiles.empty())
				param.SettingFilePath = jsonFiles[0];
		}

		if (!param.SettingFilePath.empty())
		{
			if (!Shp2ModelParam::ParseSetting(param.SettingFilePath, param))
				return false;
		}

		std::shared_ptr<ModelProduceProgress> modelProduceProgress = modelProduceProgressPtr;
		modelProduceProgress->AddProgressChapter("build rb index", 0.0, 0.05);
		modelProduceProgress->AddProgressChapter("produce model", 0.05, 0.95);
		modelProduceProgress->AddProgressChapter("write model", 0.95, 1);
		modelProduceProgress->SetExternalProgressCallback(modelProgressFunc);

		if (!Shp2Model::Convert(param, modelBuildArgs)) {
			modelProduceProgress->SetExternalProgressCallback(nullptr);
			return false;
		}

		modelProduceProgress->SetExternalProgressCallback(nullptr);
		return true;
	}
}



#ifndef EPGIS_CORE_SHP2MODEL_H 
#define EPGIS_CORE_SHP2MODEL_H 
#include <DataSet/ModelDataSet.h> 
#include <Common/ModelBuildArgs.h> 
namespace EPGIS_3D 
{ 
/** 
* @brief 墙面纹理设置 
*/ 
struct TextureParam 
{ 
std::string 
ImgFilePath; 
double 
Width; 
double 
Height; 
}; 
/** 
* @brief 建筑物纹理设置. 
*/ 
struct BuildingTextureParam 
{ 
std::string FilePath; 
std::string Filter; 
TextureParam WallTexture; 
TextureParam RoofTexture; 
}; 
/** 
* @brief 热力图设置. 
*/ 
struct HeatmapParam 
{ 
std::string 
FilePath; 
std::string 
HeatMapPOIFile; 
std::string 
LongitudeField; 
std::string 
LatitudeField; 
std::string 
HeightField; 
std::string 
IntensityField; 
std::string 
HeatmapAlgorithm; 
double 
MaxIntensity; 
double 
MinIntensity; 
double 
POIRadius; 
double 
HeatmapBlur; 
bool 
UserCustomerShader = false; 
}; 
/**2 
* @brief shp 数据生产模型参数. 
*/ 
struct Shp2ModelParam 
{ 
std::string 
ShapeFilePath; 
std::string 
OutputFilePath; 
std::string 
SettingFilePath; 
double 
DefaultHeight; 
double 
HeightFiledScale; 
std::string 
HeightField; 
std::string 
FilterField; 
int 
StartLevel; 
int 
EndLevel; 
ModelDataSetPtr 
PtrModelDataSet; 
std::vector<BuildingTextureParam> 
BuildingTextures; 
HeatmapParam 
heatmapParam; 
std::string 
FloorHeightField; 
std::string 
FloorField; 
static bool ParseSetting(const std::string& jsonFile, Shp2ModelParam& param); 
/** 
* @brief 解析热力图参数配置文件. 
* 
* @param jsonFile 配置文件路径 
* @param param 返回参数 
*/ 
static void ParseHeatmapSetting(const nlohmann::json& json, Shp2ModelParam& param); 
/** 
* @brief 解析墙面纹理配置文件. 
* 
* @param jsonFile 配置文件路径 
* @param param 返回参数 
*/ 
static void ParseBuildingTextureSetting(const nlohmann::json& json, Shp2ModelParam& param); 
}; 
/** 
* @brief shp 数据转模型. 
*/ 
class Shp2Model 
{ 
public: 
static bool Convert(const Shp2ModelParam& param,const ModelBuildArgs& buildArgs = ModelBuildArgs()); 
}; 
} // end of namespace EPGIS_3D 
#endif

#include <Common/FileSystemUtil.h> 
#include <Common/Image.h> 
#include <Common/MathUtil.h> 
#include <Common/StringUtil.h> 
#include "DataSet/ModelDataSet.h" 
#include <ogrsf_frmts.h> 
#include <Common/Extern.h> 
#include <Common/ThreadPool.h> 
#include <Common/PyramidHCurveVisitor.h> 
#include <Terrain/TBWaterMaskExtension.h> 
#include <Core/Core.h> 
#include <Model/StandardShapes.h> 
#include <Model/ModParser.h> 
#include <oce/BRepMesh_IncrementalMesh.hxx> 
#include <oce/BRepLProp_SLProps.hxx> 
#include <oce/BRepBuilderAPI_Transform.hxx> 
#include <oce/BRepBuilderAPI_MakePolygon.hxx> 
#include <oce/BRepBuilderAPI_MakeFace.hxx> 
#include <oce/TopExp_Explorer.hxx> 
#include <oce/TopoDS_Wire.hxx> 
#include <oce/Poly_Triangle.hxx> 
#include <oce/Poly_Triangulation.hxx> 
#include <oce/TColgp_Array1OfPnt2d.hxx> 
#include <oce/BRepMesh_IncrementalMesh.hxx> 
#include <oce/BRepLProp_SLProps.hxx> 
#include "ogrsf_frmts.h" 
#include "Shp2Model.h" 
#include "ModelProcessor.h" 
#include "DataSet/FileModelDataSet.h" 
#include "DataSet/ModelDataSet.h" 
#include "Terrain/TBWaterMaskExtension.h" 
#include "DataSet/ModelDataSet.h" 
#include "Tileset/Tileset.h" 
#include "Tileset/Transformation.h" 
#include <tiny_gltf.h> 
#include <csv.h> 
#include <Common/stdafx.h> 
#include <atomic> 
#include <limits> 
using namespace EPGIS_3D; 

std::string VERTEX_SHADER = R"( 
precision highp float; 
uniform mat4 u_modelViewMatrix; 
uniform mat4 u_projectionMatrix; 
uniform mat3 u_normalMatrix; 

attribute vec3 a_position; 
attribute vec3 a_normal; 
attribute vec2 a_texcoord0; 
attribute float a_batchId; 
varying vec2 v_texcoord0; 
varying vec3 v_normal; 
varying vec3 v_pos; 
varying vec3 v_position; 
void main(void) 
{ 
vec4 pos = u_modelViewMatrix * vec4(a_position, 1.0); 
v_pos = pos.xyz / pos.w; 
v_normal = u_normalMatrix * a_normal; 
v_texcoord0 = a_texcoord0; 
gl_Position = u_projectionMatrix * pos; 
v_position = a_position; 
} 
)"; 

std::string FRAGMENT_SHADER = R"( 
precision highp float; 
varying vec2 v_texcoord0; 
varying vec3 v_normal; 
varying vec3 v_pos; 
varying vec3 v_position; 
uniform sampler2D u_texImage; 
uniform vec4 u_diffuse; 
uniform vec4 u_ambient; 
uniform vec4 u_specular; 
//uniform vec4 u_emission; 
//uniform float u_shininess; 
uniform float u_light; 
uniform float x_min; 
uniform float x_max; 
uniform float y_min; 
uniform float y_max; 
uniform float z_min; 
uniform float z_max; 
uniform float i_min; 
uniform float i_max; 
uniform float width;5 
uniform float height; 
uniform float depth; 
uniform float total_max; 
uniform float total_min; 
void main(void) 
{ 
float x = (v_position.x-x_min)/(x_max-x_min); 
float y = (v_position.y-y_min)/(y_max-y_min); 
float z = (v_position.z-z_min)/(z_max-z_min); 
float len = width * height; 
float h = z * len ; 
float h_w = floor(mod(h,width / 4.0)); 
float h_h = floor(h / width); 
vec2 uv; 
uv.x = (x + h_w) * 4.0/ width; 
uv.y = 1.0 - (y + h_h)/height; 
vec4 color1 = texture2D(u_texImage, uv); 
float ff = i_max * color1.y + i_min*(1.0-color1.y); 
ff = (ff - total_min) / (total_max - total_min); 
gl_FragColor = vec4(1.0,1.0 - ff,1.0 - ff,1.0); 
} 
)"; 
struct BuildingTexture 
{ 
std::string Filter; 
BuildingTextureParam Param; 
TexturePtr PtrWallTexture; 
TexturePtr PtrRoofTexture; 
}; 

using BuildingTexturePtr = std::shared_ptr<BuildingTexture>; 
class FeatureBuilder 
{ 
static void BuildMeshByFace(VertexArrayPtr vertexArrayPtr, VertexIndexArrayPtr vertexIndexArrayPtr, 
NormalArrayPtr normalArrayPtr, TextureUVArrayPtr textureUVArrayPtr, const TopoDS_Face& face, 
const DeflectOptions& deflectOptions, int& index1, ModelPtr ptrModel,double textureWidth,double 
textureHeight) 
{ 
TopLoc_Location theLocation; 
bool isRelative = false; 
bool isParaller = true; 
BRepMesh_IncrementalMesh(face, deflectOptions.theLinDeflection, isRelative, 6 
deflectOptions.theAngDeflection, isParaller); 
const Handle_Poly_Triangulation& theTriangulation = BRep_Tool::Triangulation(face, theLocation); 
BRepLProp_SLProps theProp(BRepAdaptor_Surface(face), 1, Precision::Confusion()); 
if (theTriangulation.IsNull()) 
return; 
auto ptrModelCS = ptrModel->GetModelCSPtr(); 
Point3d first; 
for (Standard_Integer t = 1; t <= theTriangulation->NbTriangles(); ++t) 
{ 
const Poly_Triangle& theTriangle = theTriangulation->Triangles().Value(t); 
gp_Pnt theVertex1; 
gp_Pnt theVertex2; 
gp_Pnt theVertex3; 
gp_Pnt2d theUV1; 
gp_Pnt2d theUV2; 
gp_Pnt2d theUV3; 
if (face.Orientation() == TopAbs_REVERSED) { 
theVertex1 = theTriangulation->Nodes().Value(theTriangle(3)); 
theVertex2 = theTriangulation->Nodes().Value(theTriangle(2)); 
theVertex3 = theTriangulation->Nodes().Value(theTriangle(1)); 
theUV1 = theTriangulation->UVNodes().Value(theTriangle(3)); 
theUV2 = theTriangulation->UVNodes().Value(theTriangle(2)); 
theUV3 = theTriangulation->UVNodes().Value(theTriangle(1)); 
} 
else { 
theVertex1 = theTriangulation->Nodes().Value(theTriangle(1)); 
theVertex2 = theTriangulation->Nodes().Value(theTriangle(2)); 
theVertex3 = theTriangulation->Nodes().Value(theTriangle(3)); 
theUV1 = theTriangulation->UVNodes().Value(theTriangle(1)); 
theUV2 = theTriangulation->UVNodes().Value(theTriangle(2)); 
theUV3 = theTriangulation->UVNodes().Value(theTriangle(3)); 
} 
Point3d p1(theVertex1.X(), theVertex1.Y(), theVertex1.Z()); 
Point3d p2(theVertex2.X(), theVertex2.Y(), theVertex2.Z()); 
Point3d p3(theVertex3.X(), theVertex3.Y(), theVertex3.Z()); 
if (t == 1) 
first = p1; 
ptrModelCS->Geo2Model(p1.x(), p1.y(), p1.z()); 
ptrModelCS->Geo2Model(p2.x(), p2.y(), p2.z()); 
ptrModelCS->Geo2Model(p3.x(), p3.y(), p3.z());7 
vertexArrayPtr->push_back(p1.x()); 
vertexArrayPtr->push_back(p1.y()); 
vertexArrayPtr->push_back(p1.z()); 
vertexArrayPtr->push_back(p2.x()); 
vertexArrayPtr->push_back(p2.y()); 
vertexArrayPtr->push_back(p2.z()); 
vertexArrayPtr->push_back(p3.x()); 
vertexArrayPtr->push_back(p3.y()); 
vertexArrayPtr->push_back(p3.z()); 
vertexIndexArrayPtr->push_back(index1++); 
vertexIndexArrayPtr->push_back(index1++); 
vertexIndexArrayPtr->push_back(index1++); 
gp_Vec V12(theVertex1, theVertex2); 
gp_Vec V13(theVertex1, theVertex3); 
gp_Vec theNormal = V12 ^ V13; 
gp_Vec theNormal1 = theNormal; 
gp_Vec theNormal2 = theNormal; 
gp_Vec theNormal3 = theNormal; 
if (theNormal.Magnitude() > Precision::Confusion()) 
{ 
theNormal.Normalize(); 
theNormal1.Normalize(); 
theNormal2.Normalize(); 
theNormal3.Normalize(); 
} 
theProp.SetParameters(theUV1.X(), theUV1.Y()); 
if (theProp.IsNormalDefined()) 
{ 
theNormal1 = theProp.Normal(); 
} 
theProp.SetParameters(theUV2.X(), theUV2.Y()); 
if (theProp.IsNormalDefined()) 
{ 
theNormal2 = theProp.Normal(); 
} 
theProp.SetParameters(theUV3.X(), theUV3.Y()); 
if (theProp.IsNormalDefined()) 
{ 
theNormal3 = theProp.Normal(); 
} 
if (face.Orientation() == TopAbs_REVERSED) 
{
theNormal.Reverse(); 
theNormal1.Reverse(); 
theNormal2.Reverse(); 
theNormal3.Reverse(); 
} 
normalArrayPtr->push_back(theNormal1.X()); 
normalArrayPtr->push_back(theNormal1.Y()); 
normalArrayPtr->push_back(theNormal1.Z()); 
normalArrayPtr->push_back(theNormal2.X()); 
normalArrayPtr->push_back(theNormal2.Y()); 
normalArrayPtr->push_back(theNormal2.Z()); 
normalArrayPtr->push_back(theNormal3.X()); 
normalArrayPtr->push_back(theNormal3.Y()); 
normalArrayPtr->push_back(theNormal3.Z()); 
/* 
textureUVArrayPtr->push_back(theUV1.X() / 10); 
textureUVArrayPtr->push_back(theUV1.Y() / 10); 
textureUVArrayPtr->push_back(theUV2.X() / 10); 
textureUVArrayPtr->push_back(theUV2.Y() / 10); 
textureUVArrayPtr->push_back(theUV3.X() / 10); 
textureUVArrayPtr->push_back(theUV3.Y() / 10);*/ 
textureUVArrayPtr->push_back(fabs(p1.x() - first.x()) / textureWidth); 
textureUVArrayPtr->push_back(fabs(p1.y() - first.y()) / textureHeight); 
textureUVArrayPtr->push_back(fabs(p2.x() - first.x()) / textureWidth); 
textureUVArrayPtr->push_back(fabs(p2.y() - first.y()) / textureHeight); 
textureUVArrayPtr->push_back(fabs(p3.x() - first.x()) / textureWidth); 
textureUVArrayPtr->push_back(fabs(p3.y() - first.y()) / textureHeight); 
} 
} 
static void BuildMeshByShape(VertexArrayPtr vertexArrayPtr, VertexIndexArrayPtr vertexIndexArrayPtr, 
NormalArrayPtr normalArrayPtr, TextureUVArrayPtr textureUVArrayPtr, 
glm::dmat4 tranformMatrix, TopoDS_Shape& shape, const DeflectOptions& deflectOptions, int& 
index1, 
ModelPtr modelPtr,double textureWidht,double textureHeight) 
{ 
gp_Trsf aLoc; 
aLoc.SetValues(tranformMatrix[0][0], tranformMatrix[0][1], tranformMatrix[0][2], 
tranformMatrix[0][3], 
tranformMatrix[1][0], tranformMatrix[1][1], tranformMatrix[1][2], tranformMatrix[1][3], 
tranformMatrix[2][0], tranformMatrix[2][1], tranformMatrix[2][2], tranformMatrix[2][3]); 
BRepBuilderAPI_Transform aBRepTrsf(shape, aLoc, true); 
shape = aBRepTrsf.Shape(); 
for (TopExp_Explorer aFaceExplorer(shape, TopAbs_FACE); aFaceExplorer.More(); aFaceExplorer.Next()) 
{ 
TopoDS_Face aFace = TopoDS::Face(aFaceExplorer.Current());9 
BuildMeshByFace(vertexArrayPtr, vertexIndexArrayPtr, normalArrayPtr, textureUVArrayPtr, 
aFace, deflectOptions, index1, modelPtr, textureWidht,textureHeight); 
} 
} 
static bool Convert2Shape(OGRPolygon* polygon, double h, double height, TopoDS_Shape& shape) 
{ 
std::vector<double> normals{ 0,0,1 }; 
OGRLinearRing* ring = polygon->getExteriorRing(); 
int num = ring->getNumPoints(); 
std::vector<double> coords; 
for (int i = 0; i < num; ++i) 
{ 
OGRPoint pt; 
ring->getPoint(i, &pt); 
coords.push_back(pt.getX()); 
coords.push_back(pt.getY()); 
coords.push_back(h); 
} 
shape = StandardShapes::MakeStretchedBody(normals, coords, height); 
return true; 
} 

static bool BuildWall(const std::vector<double>& coords, double height, ModelPtr modelPtr, 
BuildingTexturePtr ptrBuildingTexture,float batchid) 
{ 
auto ptrModelCS = modelPtr->GetModelCSPtr(); 
VertexArrayPtr 
ptrVertexArrayPtr = std::make_shared<VertexArray>(); 
VertexIndexArrayPtr 
ptrVertexIndexArrayPtr = std::make_shared<VertexIndexArray>(); 
NormalArrayPtr 
ptrNormalArrayPtr = std::make_shared<NormalArray>(); 
TextureUVArrayPtr ptrTextureUVArray = std::make_shared<TextureUVArray>(); 
BatchIDArrayPtr ptrBatchIDArray = std::make_shared<BatchIDArray>(); 
MeshPtr ptrMesh = std::make_shared<Mesh>(); 
ptrMesh->SetVertexArrayPtr(ptrVertexArrayPtr); 
ptrMesh->SetVertexIndexArrayPtr(ptrVertexIndexArrayPtr); 
ptrMesh->SetNormalArrayPtr(ptrNormalArrayPtr); 
ptrMesh->SetTextureUVArrayPtr(0,ptrTextureUVArray); 
ptrMesh->SetBatchIDArrayPtr(ptrBatchIDArray); 
int num = coords.size() / 3; 
int faceCount = (num - 1) * 2; 
ptrVertexArrayPtr->reserve(faceCount*6); 
ptrVertexIndexArrayPtr->reserve(faceCount * 3); 
ptrNormalArrayPtr->reserve(faceCount * 3); 
ptrTextureUVArray->reserve(faceCount * 4); 
ptrBatchIDArray->reserve(faceCount * 2); 
double textureWidht = 10.0; 
double textureHeight = 15.0;10 
if (ptrBuildingTexture && ptrBuildingTexture->PtrWallTexture != nullptr) 
{ 
textureWidht = ptrBuildingTexture->Param.WallTexture.Width; 
textureHeight = ptrBuildingTexture->Param.WallTexture.Height; 
} 
for (int i = 0; i < num - 1; ++i) 
{ 
glm::dvec3 start{ coords[i * 3] , coords[i * 3 + 1], coords[i * 3 + 2] }; 
glm::dvec3 end{ coords[i * 3 + 3] , coords[i * 3 + 4], coords[i * 3 + 5] }; 
glm::dvec3 topstart = start; 
topstart.z += height; 
glm::dvec3 topend = end; 
topend.z += height; 
ptrModelCS->Geo2Model(start.x, start.y, start.z); 
ptrModelCS->Geo2Model(end.x, end.y, end.z); 
ptrModelCS->Geo2Model(topstart.x, topstart.y, topstart.z); 
ptrModelCS->Geo2Model(topend.x, topend.y, topend.z); 
double distance = glm::distance(start, end); 
double uf = distance / textureWidht; 
double vf = height / textureHeight; 
ptrVertexArrayPtr->push_back(start.x); 
ptrVertexArrayPtr->push_back(start.y); 
ptrVertexArrayPtr->push_back(start.z); 
ptrVertexArrayPtr->push_back(end.x); 
ptrVertexArrayPtr->push_back(end.y); 
ptrVertexArrayPtr->push_back(end.z); 
ptrVertexArrayPtr->push_back(topstart.x); 
ptrVertexArrayPtr->push_back(topstart.y); 
ptrVertexArrayPtr->push_back(topstart.z); 
ptrVertexArrayPtr->push_back(topend.x); 
ptrVertexArrayPtr->push_back(topend.y); 
ptrVertexArrayPtr->push_back(topend.z); 
ptrBatchIDArray->push_back(batchid); 
ptrBatchIDArray->push_back(batchid); 
ptrBatchIDArray->push_back(batchid); 
ptrBatchIDArray->push_back(batchid); 
ptrTextureUVArray->push_back(0); 
ptrTextureUVArray->push_back(0); 
ptrTextureUVArray->push_back(uf); 
ptrTextureUVArray->push_back(0); 
ptrTextureUVArray->push_back(0); 
ptrTextureUVArray->push_back(vf); 
ptrTextureUVArray->push_back(uf); 
ptrTextureUVArray->push_back(vf);11 
ptrVertexIndexArrayPtr->push_back(i * 4 + 1); 
ptrVertexIndexArrayPtr->push_back(i * 4); 
ptrVertexIndexArrayPtr->push_back(i * 4 + 2); 
ptrVertexIndexArrayPtr->push_back(i * 4 + 1); 
ptrVertexIndexArrayPtr->push_back(i * 4 + 2); 
ptrVertexIndexArrayPtr->push_back(i * 4 + 3); 
glm::dvec3 xv = end - start; 
glm::dvec3 yv = topstart - start; 
auto normal = glm::normalize(glm::cross(xv, yv)); 
ptrNormalArrayPtr->push_back(normal.x); 
ptrNormalArrayPtr->push_back(normal.y); 
ptrNormalArrayPtr->push_back(normal.z); 
ptrNormalArrayPtr->push_back(normal.x); 
ptrNormalArrayPtr->push_back(normal.y); 
ptrNormalArrayPtr->push_back(normal.z); 
ptrNormalArrayPtr->push_back(normal.x); 
ptrNormalArrayPtr->push_back(normal.y); 
ptrNormalArrayPtr->push_back(normal.z); 
ptrNormalArrayPtr->push_back(normal.x); 
ptrNormalArrayPtr->push_back(normal.y); 
ptrNormalArrayPtr->push_back(normal.z); 
} 
std::shared_ptr<aiMaterial> imgMaterial = std::make_shared<aiMaterial>(); 
InitAssimpMaterial(imgMaterial.get(),0,0); 
aiColor4D baseColorFactor = aiColor4D(ai_real(1.0), ai_real(1.0), ai_real(1.0), ai_real(1.0)); 
imgMaterial->AddProperty<aiColor4D>(&baseColorFactor, 1, 
AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_FACTOR); 
const int two_sided = 0; 
imgMaterial->AddProperty(&two_sided, 1, AI_MATKEY_TWOSIDED); 
if (ptrBuildingTexture != nullptr && ptrBuildingTexture->PtrWallTexture != nullptr) 
{ 
ptrMesh->SetTexturePtr(0, ptrBuildingTexture->PtrWallTexture); 
aiString materialName(ptrBuildingTexture->PtrWallTexture->GetName()); 
double roughness = 0; 
double metalness = 0; 
float opacity = 0.5; 
imgMaterial->AddProperty(&opacity, 1, AI_MATKEY_OPACITY); 
if (opacity < 1.0) { 
aiString alphaMode("BLEND"); 
imgMaterial->AddProperty(&alphaMode, AI_MATKEY_GLTF_ALPHAMODE); 
} 
imgMaterial->AddProperty(&materialName, AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0)); 
imgMaterial->AddProperty(&metalness, 1, 
AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR);12 
imgMaterial->AddProperty(&roughness, 1, 
AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR); 
} 
modelPtr->AddMesh(ptrMesh); 
ptrMesh->SetAssimpMaterial(imgMaterial); 
return true; 
} 

static bool BuildRoof(const std::vector<double>& coords, double height, ModelPtr modelPtr, 
BuildingTexturePtr ptrBuildingTexture,float batchid) 
{ 
BRepBuilderAPI_MakePolygon polygon; 
//Real z = 0.0; 
int pointSize = coords.size() / 3; 
for (int i = 0; i < pointSize; i++) 
{ 
//不在一个平面，程序崩溃 
polygon.Add(gp_Pnt(coords[3 * i], coords[3 * i + 1], coords[3 * i + 2] + height)); 
} 
//如果首尾点不同,需要闭合 
polygon.Close(); 
TopoDS_Wire wire = polygon.Wire(); 
TopoDS_Face shape = BRepBuilderAPI_MakeFace(wire); 
shape.Orientation(TopAbs_Orientation::TopAbs_REVERSED); 
if (shape.IsNull()) 
return false; 
MeshPtr meshPtr = std::make_shared<Mesh>(); 
VertexArrayPtr vertexArrayPtr = meshPtr->GetVertexArrayPtr(); 
if (!vertexArrayPtr) { 
vertexArrayPtr = std::make_shared<VertexArray>(); 
}
VertexIndexArrayPtr vertexIndexArrayPtr = meshPtr->GetVertexIndexArray(); 
if (!vertexIndexArrayPtr) { 
vertexIndexArrayPtr = std::make_shared<VertexIndexArray>(); 
} 
NormalArrayPtr normalArrayPtr = meshPtr->GetNormalArrayPtr(); 
if (!normalArrayPtr) { 
normalArrayPtr = std::make_shared<NormalArray>(); 
} 
TextureUVArrayPtr textureUVArrayPtr = meshPtr->GetTextureUVArrayPtr(0); 
if (!textureUVArrayPtr) { 
textureUVArrayPtr = std::make_shared<TextureUVArray>(); 
}
int index1 = 0; 
DeflectOptions deflectOptions; 
deflectOptions.theLinDeflection = 0.1f; 
deflectOptions.theAngDeflection = 1.0f; 
glm::dmat4 tranformMatrix(1.0); 
tranformMatrix = glm::transpose(tranformMatrix); 
double textureWidth = 10.0; 
double textureHeight = 10.0; 
if (ptrBuildingTexture && ptrBuildingTexture->PtrRoofTexture) 
{ 
textureWidth = ptrBuildingTexture->Param.RoofTexture.Width; 
textureWidth = ptrBuildingTexture->Param.RoofTexture.Height; 
} 
BuildMeshByShape(vertexArrayPtr, vertexIndexArrayPtr, normalArrayPtr, textureUVArrayPtr, 
tranformMatrix, shape, deflectOptions, index1, modelPtr, textureWidth, textureHeight); 
std::shared_ptr<aiMaterial> copiedMaterial = std::make_shared<aiMaterial>(); 
InitAssimpMaterial(copiedMaterial.get(),0,0); 
aiColor4D baseColorFactor = aiColor4D(ai_real(1.0), ai_real(1.0), ai_real(1.0), ai_real(1.0)); 
copiedMaterial->AddProperty<aiColor4D>(&baseColorFactor, 1, 
AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_FACTOR); 
const int two_sided = 0; 
copiedMaterial->AddProperty(&two_sided, 1, AI_MATKEY_TWOSIDED); 
if (ptrBuildingTexture && ptrBuildingTexture->PtrRoofTexture != nullptr) 
{ 
meshPtr->SetTexturePtr(0, ptrBuildingTexture->PtrRoofTexture); 
aiString materialName(ptrBuildingTexture->PtrRoofTexture->GetName()); 
double roughness = 0; 
double metalness = 0; 
float opacity = 0.5; 
copiedMaterial->AddProperty(&opacity, 1, AI_MATKEY_OPACITY); 
if (opacity < 1.0) { 
aiString alphaMode("BLEND"); 
copiedMaterial->AddProperty(&alphaMode, AI_MATKEY_GLTF_ALPHAMODE); 
} 
copiedMaterial->AddProperty(&materialName, AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 
0)); 
copiedMaterial->AddProperty(&metalness, 1, 
AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR); 
copiedMaterial->AddProperty(&roughness, 1, 
AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR); 
} 
meshPtr->SetVertexArrayPtr(vertexArrayPtr); 
meshPtr->SetNormalArrayPtr(normalArrayPtr);14 
meshPtr->SetTextureUVArrayPtr(0, textureUVArrayPtr); 
meshPtr->SetVertexIndexArrayPtr(vertexIndexArrayPtr); 
meshPtr->SetAssimpMaterial(copiedMaterial); 
int num = vertexArrayPtr->size() / 3; 
BatchIDArrayPtr ptrBatchIDArray = std::make_shared<BatchIDArray>(num, batchid); 
meshPtr->SetBatchIDArrayPtr(ptrBatchIDArray); 
modelPtr->AddMesh(meshPtr); 
modelPtr->SetTotalVertices(meshPtr->GetVertexArrayPtr()->size()); 
return true; 
} 
static bool Convert2Mesh(OGRPolygon* polygon, double h, double height, ModelPtr modelPtr, 
BuildingTexturePtr ptrBuildingTexture) 
{ 
std::vector<double> normals{ 0,0,1 }; 
OGRLinearRing* ring = polygon->getExteriorRing(); 
if (ring == nullptr) 
return false; 
int num = ring->getNumPoints(); 
std::vector<double> coords; 
for (int i = 0; i < num; ++i) 
{ 
OGRPoint pt; 
ring->getPoint(i, &pt); 
coords.push_back(pt.getX()); 
coords.push_back(pt.getY()); 
coords.push_back(h); 
} 
BuildWall(coords, height, modelPtr, ptrBuildingTexture,0); 
BuildRoof(coords, height, modelPtr, ptrBuildingTexture,0); 
return true; 
} 
static bool Convert2MeshByFloor(OGRPolygon* polygon, int floor,double h, double floorHeight, ModelPtr 
modelPtr, BuildingTexturePtr ptrBuildingTexture) 
{ 
std::vector<double> normals{ 0,0,1 }; 
OGRLinearRing* ring = polygon->getExteriorRing(); 
if (ring == nullptr) 
return false; 
int num = ring->getNumPoints(); 
std::vector<double> coords; 
for (int i = 0; i < num; ++i) 
{ 
OGRPoint pt; 
ring->getPoint(i, &pt); 
coords.push_back(pt.getX());15 
coords.push_back(pt.getY()); 
coords.push_back(h); 
} 
for (int i = 0; i < floor; ++i) 
{ 
for (int j = 0; j < num; ++j) 
{ 
coords[j * 3 + 2] = i * floorHeight + h; 
} 
BuildWall(coords, floorHeight, modelPtr, ptrBuildingTexture,static_cast<float>(i)); 
} 
BuildRoof(coords, floorHeight, modelPtr, ptrBuildingTexture, static_cast<float>(floor)); 
return true; 
} 
static double QueryHeight(double longitude, double latitude) { 
TerrainHeightQuery* terrainHeightQuery = TerrainHeightQuery::GetSingletonPtr(); 
if (!terrainHeightQuery) { 
return 0; 
} 
return terrainHeightQuery->QueryHeight(longitude, latitude); 
} 
public: 
static ModelPtr Convert2Model(const FeaturePtr& ptrFea, double height, BuildingTexturePtr ptrTexture = 
nullptr) 
{ 
OGRGeometry* pGeom = ptrFea->GetGeometryRef(); 
if (pGeom == nullptr) 
return nullptr; 
int dim = pGeom->getDimension(); 
if (dim != 2) 
return nullptr; 
OGRwkbGeometryType type = pGeom->getGeometryType(); 
std::vector<double> normals{ 0,0,1 }; 
ModelPtr ptrModel = std::make_shared<Model>(); 
TopoDS_Shape shape; 
OGREnvelope3D envelop; 
pGeom->getEnvelope(&envelop); 
double l = (envelop.MaxX + envelop.MinX) / 2; 
double b = (envelop.MaxY + envelop.MaxY) / 2; 
double h = QueryHeight(l, b); 
ptrModel->SetLongitude((envelop.MaxX + envelop.MinX) / 2); 
ptrModel->SetLatitude((envelop.MaxY + envelop.MaxY) / 2); 
ptrModel->SetHeight(h);
switch (type) 
{ 
case OGRwkbGeometryType::wkbPolygon: 
{ 
OGRPolygon* polygon = static_cast<OGRPolygon*>(pGeom->Simplify(FLT_EPSILON)); 
//if (!Convert2Shape(polygon, h, height, shape)) 
// 
return nullptr; 
if (polygon == nullptr) 
return nullptr; 
if (!Convert2Mesh(polygon, h, height, ptrModel, ptrTexture)) 
return nullptr; 
auto& attrs = ptrModel->GetAttributes(); 
attrs["fid"][0] = std::to_string(ptrFea->GetID()); 
attrs["height"][0] = std::to_string(height); 
delete polygon; 
} 
break; 
default: 
return nullptr; 
} 
//Convert2Model(shape, ptrModel); 
ptrModel->DirtyBound(); 
return ptrModel; 
} 
static ModelPtr Convert2ModelByFloor(const FeaturePtr& ptrFea,int floor, double floorHeight, 
BuildingTexturePtr ptrTexture = nullptr) 
{ 
OGRGeometry* pGeom = ptrFea->GetGeometryRef(); 
if (pGeom == nullptr) 
return nullptr; 
int dim = pGeom->getDimension(); 
if (dim != 2) 
return nullptr; 
OGRwkbGeometryType type = pGeom->getGeometryType(); 
std::vector<double> normals{ 0,0,1 }; 
ModelPtr ptrModel = std::make_shared<Model>(); 
TopoDS_Shape shape; 
OGREnvelope3D envelop; 
pGeom->getEnvelope(&envelop); 
double l = (envelop.MaxX + envelop.MinX) / 2; 
double b = (envelop.MaxY + envelop.MaxY) / 2; 
double h = QueryHeight(l, b); 
ptrModel->SetLongitude((envelop.MaxX + envelop.MinX) / 2); 
ptrModel->SetLatitude((envelop.MaxY + envelop.MaxY) / 2); 
ptrModel->SetHeight(h);
switch (type) 
{ 
case OGRwkbGeometryType::wkbPolygon: 
{ 
OGRPolygon* polygon = static_cast<OGRPolygon*>(pGeom->Simplify(FLT_EPSILON)); 
//if (!Convert2Shape(polygon, h, height, shape)) 
// 
return nullptr; 
if (polygon == nullptr) 
return nullptr; 
if (!Convert2MeshByFloor(polygon, floor, h, floorHeight, ptrModel, ptrTexture)) 
return nullptr; 
auto& attrs = ptrModel->GetAttributes(); 
for (int i = 0; i < floor; ++i) 
{ 
attrs["fid"][i] = std::to_string(ptrFea->GetID()); 
attrs["floorHeight"][i] = std::to_string(floorHeight); 
attrs["floor"][i] = std::to_string(i); 
} 
delete polygon; 
} 
break; 
default: 
break; 
} 
//Convert2Model(shape, ptrModel); 
ptrModel->DirtyBound(); 
return ptrModel; 
} 
}; 

struct Shp2ModelModelData 
{ 
FeatureIndex* pFeaIndex; 
ModelPtr
PtrModel; 
double
Area; 
}; 
struct Shp2ModelTileData 
{ 
unsigned int 
Level; 
PyramidKey 
Key; 
TilesetNode 
Node; 
ModelCoordinateSystemPtr 
ModelCS; 
BoundingVolumeRegion 
Region; 
std::vector<Shp2ModelModelData> Models; 
}; 

typedef std::shared_ptr<Shp2ModelTileData> Shp2ModelTileDataPtr;18 
class Shp2ModelTileByLevel 
{ 
public: 
Shp2ModelTileByLevel(int level, int minRow, int minCol, int maxRow, int maxCol) 
:m_Level(level), m_MinRow(minRow), m_MaxRow(maxRow), m_MinCol(minCol), m_MaxCol(maxCol) 
{
} 
int Level() const 
{ 
return m_Level; 
} 
std::function<bool(const std::vector<Shp2ModelTileDataPtr>& vecNode, const PyramidKey&)> 
MergeTaskFunc; 
void Add(Shp2ModelTileDataPtr ptrData) 
{ 
{ 
std::lock_guard<std::mutex> lock(mMapMutex); 
mChildMap[ptrData->Key] = ptrData; 
} 
ProcessMerge(ptrData->Key.Parent()); 
} 
void Add(const PyramidKey& key) 
{ 
{ 
std::lock_guard<std::mutex> lock(mMapMutex); 
mChildMap[key] = NULL; 
} 
ProcessMerge(key.Parent()); 
} 
private: 
int m_Level, m_MinRow, m_MaxRow, m_MinCol, m_MaxCol; 
std::map<PyramidKey, Shp2ModelTileDataPtr> mChildMap; 
void ProcessMerge(const PyramidKey& key) 
{ 
if (!IsPrepareForMerge(key)) 
return; 
std::vector<Shp2ModelTileDataPtr> vec; 
for (const auto& k : key.Children()) 
{ 
auto child = Child(k); 
if (child.get() == nullptr) 
continue; 
vec.push_back(child); 
} 
if (MergeTaskFunc) 
MergeTaskFunc(vec, key);19 
} 
bool ExistChild(const PyramidKey& key) 
{ 
std::lock_guard<std::mutex> lock(mMapMutex); 
if (mChildMap.find(key) != mChildMap.end()) 
return true; 
if (IsOutOfRange(key)) 
return true; 
return false; 
} 
bool IsOutOfRange(const PyramidKey& key) 
{ 
if ((int)key.Row<m_MinRow || (int)key.Row>m_MaxRow || (int)key.Col<m_MinCol || 
(int)key.Col>m_MaxCol) 
return true; 
return false; 
} 
bool IsPrepareForMerge(const PyramidKey& key) 
{ 
int n = 0; 
for (const auto& k : key.Children()) 
{ 
if (ExistChild(k)) 
++n; 
} 
if (n < 4) 
return false; 
return true; 
} 
Shp2ModelTileDataPtr Child(const PyramidKey& key) 
{ 
std::lock_guard<std::mutex> lock(mMapMutex); 
auto it = mChildMap.find(key); 
if (it == mChildMap.end()) 
return nullptr; 
auto ptr = it->second; 
mChildMap.erase(it); 
return ptr; 
} 
std::mutex mMapMutex; 
}; 
using Shp2ModelTileByLevelPtr = std::shared_ptr<Shp2ModelTileByLevel>; 
static inline double FloatClamp(double v, double start, double end) 
{ 
if (v < 0) 
return end; 
if (v > 1) 
return start; 
return start * v + end * (1 - v);20 
} 
struct POI 
{ 
double LBH[3]; 
double& X() { return LBH[0]; } 
double& Y() { return LBH[1]; } 
double& Z() { return LBH[2]; } 
//信号强度 
double Intensity; 
}; 
using POIPtr = std::shared_ptr<POI>; 
struct CubeSampler 
{ 
glm::dvec3 Min; 
glm::dvec3 Ptx; 
glm::dvec3 Pty; 
glm::dvec3 Ptz; 
TransformationPtr mPtrTransformation; 
CubeSampler(const BoundingVolumeRegion& region) 
{ 
Min = glm::vec3(region.Data[0], region.Data[1], region.Data[4]); 
Ptx = glm::vec3(region.Data[2], region.Data[1], region.Data[4]); 
Pty = glm::vec3(region.Data[0], region.Data[3], region.Data[4]); 
Ptz = glm::vec3(region.Data[0], region.Data[1], region.Data[5]); 
mPtrTransformation = std::make_shared<Geo2GeocentricTransformation>(); 
mPtrTransformation->Transform(Min.x, Min.y, Min.z); 
mPtrTransformation->Transform(Ptx.x, Ptx.y, Ptx.z); 
mPtrTransformation->Transform(Pty.x, Pty.y, Pty.z); 
mPtrTransformation->Transform(Ptz.x, Ptz.y, Ptz.z); 
Ptx = Ptx - Min; 
Pty = Pty - Min; 
Ptz = Ptz - Min; 
} 
void Sample(double& x, double& y, double& z) 
{ 
glm::vec3 pt = Min + Ptx * x + Pty * y + Ptz * z; 
x = pt.x; 
y = pt.y; 
z = pt.z; 
} 
bool HasPoint(const RTree<POIPtr, double, 3>& index,double distance) 
{ 
auto xx = glm::normalize(Ptx) * (2 * distance) + Ptx; 
auto yy = glm::normalize(Pty) * (2 * distance) + Pty;21 
auto zz = glm::normalize(Ptz) * (2 * distance) + Pty; 
auto min = Min - distance * glm::normalize(Ptx) - distance * glm::normalize(Ptx) - distance * 
glm::normalize(Ptx); 
auto max = min + xx + yy + zz; 
BoundingVolumeRegion box; 
for (int i = 0; i < 2; ++i) 
{ 
auto px = static_cast<double>(i) * xx; 
for (int j = 0;j < 2; ++j) 
{ 
auto py = static_cast<double>(j) * yy; 
for (int k = 0; k < 2; ++k) 
{ 
auto pz = static_cast<double>(k) * zz; 
auto pt = min + px + py + pz; 
box.Union(Point3d(pt.x, pt.y, pt.z)); 
} 
} 
} 
std::vector<double> vecMin{ box.Data[0],box.Data[1],box.Data[4] }; 
std::vector<double> vecMax{ box.Data[2],box.Data[3],box.Data[5] }; 
std::vector<POIPtr> res; 
int n = index.Search(vecMin.data(), vecMax.data(), [&res](const POIPtr& ptr) { 
res.push_back(ptr); 
return false; 
}); 
return !res.empty(); 
} 
glm::dvec3 Max() 
{ 
return Min + Ptx + Pty + Ptz; 
} 
}; 
struct IntensityAlgorithm 
{ 
double operator()(const std::vector<POIPtr>& pois, const Point3d& center, double r) 
{ 
double iny = 0; 
double sumDistance = 0; 
double sumIntensity = 0; 
for (auto pt : pois) 
{22 
double xx = std::fabs(pt->X() - center.x()); 
double yy = std::fabs(pt->Y() - center.y()); 
double zz = std::fabs(pt->Z() - center.z()); 
double d = sqrt(xx * xx + yy * yy + zz * zz); 
if (d > r) 
continue; 
d = 1 / d; 
sumDistance += d; 
sumIntensity += d * pt->Intensity; 
} 
if (sumDistance == 0) 
return 0; 
return sumIntensity / sumDistance; 
} 
}; 
struct IntensityDistanceAlgorithm 
{ 
IntensityDistanceAlgorithm(double radiu, double blur) 
{ 
Radiu = radiu; 
Blur = blur; 
InnerRadiu = radiu * (1 - Blur); 
OuterRadiu = radiu * Blur; 
} 
double Radiu; 
double Blur; 
double InnerRadiu; 
double OuterRadiu; 
double operator()(const std::vector<POIPtr>& pois, const Point3d& center) 
{ 
double sumIntensity = 0; 
for (auto pt : pois) 
{ 
double xx = std::fabs(pt->X() - center.x()); 
double yy = std::fabs(pt->Y() - center.y()); 
double zz = std::fabs(pt->Z() - center.z()); 
double d = sqrt(xx * xx + yy * yy + zz * zz); 
if (d > Radiu) 
continue; 
double inty = 0; 
if (d < InnerRadiu) 
inty = pt->Intensity; 
else 
{ 
inty = (1 - (d - InnerRadiu) / OuterRadiu) * pt->Intensity; 
} 
if (sumIntensity < inty) 
sumIntensity = inty;23 
} 
return sumIntensity; 
} 
}; 
class HeatMapPOI : public std::enable_shared_from_this<HeatMapPOI> 
{ 
public: 
static std::shared_ptr<HeatMapPOI> Create(const HeatmapParam& heatmapParam) 
{ 
std::shared_ptr<HeatMapPOI> res; 
res.reset(new HeatMapPOI(heatmapParam)); 
if (!res->InitPOIIndexTree()) 
return nullptr; 
return res; 
} 
HeatMapPOI(const HeatmapParam& heatmapParam) 
:mHeatmapParam(heatmapParam), mTextureSize(128) 
{ 
mPtrTransformation = std::make_shared<Geo2GeocentricTransformation>(); 
mMaxIntensity = 0; 
mMinIntensity = FLT_MAX; 
if (mHeatmapParam.HeatmapAlgorithm == "intensity") 
mHeatmapAlgo = IntensityDistanceAlgorithm(mHeatmapParam.POIRadius, 
mHeatmapParam.HeatmapBlur); 
} 
virtual ~HeatMapPOI() {} 
TexturePtr EncodeIntensity(const std::vector<double>& intensityVec, int width, int height, double min, 
double max) 
{ 
int channels = 4; 
int size = intensityVec.size(); 
unsigned char* pData = new unsigned char[size]; 
double delta = max - min; 
if (Math::floatEqual(delta, 0)) 
{ 
memset(pData, 0, size); 
} 
else 
{ 
for (int i = 0; i < width; ++i) 
{ 
for (int j = 0; j < height; ++j) 
{ 
double inty = intensityVec[i + j * width];24 
unsigned char u = 0; 
if (inty > 0) 
{ 
u = (inty - min) / delta * 255; 
} 
pData[i + j * width ] = u; 
} 
} 
} 
std::string ss; 
Image::WriteToMemory("texture.png", width/ channels, height, channels, pData, ss); 
delete[] pData; 
unsigned char* pTarData = new unsigned char[ss.size()]; 
memcpy(pTarData, ss.data(), ss.size()); 
TexturePtr ptrTexture = std::make_shared<Texture>(); 
ptrTexture->SetWidth(ss.size()); 
ptrTexture->SetHeight(0); 
ptrTexture->SetFormat(0); 
ptrTexture->SetSize(ss.size()); 
ptrTexture->SetData(pTarData); 
return ptrTexture; 
} 
TexturePtr RebuildTextures(const BoundingVolumeRegion& region, int& w, int& h, int& len, double& max, 
double& min, 
std::function<double(const std::vector<POIPtr>&, const Point3d&)> IntensityAlgo) 
{ 
CubeSampler sampler(region); 
CalcuteHeightSize(region, w, h); 
int width = mTextureSize * w; 
int height = mTextureSize * h; 
len = w * h; 
int size = width * height; 
std::vector<double> intensityVec(size, 0); 
int stride = width; 
std::atomic<bool> hasHeatPOI; 
hasHeatPOI.store(false); 
std::atomic<double> minIntensity; 
minIntensity.store(std::numeric_limits<double>::max()); 
std::atomic<double> maxIntensity; 
maxIntensity.store(std::numeric_limits<double>::min()); 
{ 
ThreadPool<> threadPool; 
std::mutex mtx; 
int channels = 4; 
for (int k = 0; k < len; k++) 
{ 
int ww = k % w; 
int ww_i = ww / channels;25 
int ww_j = ww % channels; 
int x_off = (ww_i * channels) * mTextureSize + ww_j; 
int y_off = (k / w) * mTextureSize * stride; 
for (int i = 0; i < mTextureSize; ++i) 
{ 
for (int j = 0; j < mTextureSize; ++j) 
{ 
//threadPool.enqueue([&mtx, &intensityVec, &sampler, this, len, i, j, k, x_off, 
y_off, stride, &hasHeatPOI, &IntensityAlgo, &minIntensity, &maxIntensity]() { 
Point3d center; 
center.x() = static_cast<double>(i + 0.5) / static_cast<double>(mTextureSize); 
center.y() = 1 - static_cast<double>(j + 0.5) / static_cast<double>(mTextureSize); 
center.z() = (static_cast<double>(k + 0.5)) / static_cast<double>(len); 
sampler.Sample(center.x(), center.y(), center.z()); 
auto pois = Search(center.x(), center.y(), center.z(), mHeatmapParam.POIRadius); 
if (pois.empty()) 
continue; 
double inty = IntensityAlgo(pois, center); 
hasHeatPOI = true; 
if (minIntensity > inty) 
minIntensity = inty; 
if (maxIntensity < inty) 
maxIntensity = inty; 
intensityVec[x_off + y_off + j * stride + i * channels] = inty; 
//}); 
} 
} 
} 
threadPool.Stop(); 
max = maxIntensity; 
min = minIntensity; 
} 
if (!hasHeatPOI) 
{ 
max = 0; 
min = 0; 
//return nullptr; 
} 
return EncodeIntensity(intensityVec, width, height, min, max); 
} 
void Processor(ModelPtr ptrModel, int& width, int& height, int& depth, double& max, double& min) 
{ 
auto geoRegion = ptrModel->GetGeoBoundingBox(); 
auto ptrTexture = RebuildTextures(geoRegion, width, height, depth, max, min, mHeatmapAlgo); 
if (ptrTexture == nullptr) 
return;26 
std::shared_ptr<aiMaterial> imgMaterial = std::make_shared<aiMaterial>(); 
aiColor4D diffuse; 
imgMaterial->AddProperty<aiColor4D>(&diffuse, 1, AI_MATKEY_COLOR_DIFFUSE); 
aiColor4D specular(ai_real(1.0), ai_real(1.0), ai_real(1.0), ai_real(1.0)); 
imgMaterial->AddProperty<aiColor4D>(&specular, 1, AI_MATKEY_COLOR_SPECULAR); 
aiColor4D ambient = aiColor4D(ai_real(0.5), ai_real(0.5), ai_real(0.5), ai_real(1.0)); 
imgMaterial->AddProperty<aiColor4D>(&ambient, 1, AI_MATKEY_COLOR_AMBIENT); 
aiColor4D baseColorFactor = aiColor4D(ai_real(1.0), ai_real(1.0), ai_real(1.0), ai_real(1.0)); 
imgMaterial->AddProperty<aiColor4D>(&baseColorFactor, 1, 
AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_FACTOR); 
const int two_sided = 0; 
imgMaterial->AddProperty(&two_sided, 1, AI_MATKEY_TWOSIDED); 
double roughness = 0.3; 
double metalness = 0.4; 
float opacity = 0.5; 
imgMaterial->AddProperty(&opacity, 1, AI_MATKEY_OPACITY); 
if (opacity < 1.0) { 
aiString alphaMode("BLEND"); 
imgMaterial->AddProperty(&alphaMode, AI_MATKEY_GLTF_ALPHAMODE); 
} 
aiString materialName(ptrTexture->GetName()); 
imgMaterial->AddProperty(&materialName, AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0)); 
imgMaterial->AddProperty(&metalness, 1, 
AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR); 
imgMaterial->AddProperty(&roughness, 1, 
AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR); 
for (auto ptrMesh : ptrModel->GetMeshArray()) 
{ 
ptrMesh->SetTexturePtr(0, ptrTexture); 
ptrMesh->SetAssimpMaterial(imgMaterial); 
} 
} 
double GetMaxIntensity() 
{ 
return mMaxIntensity; 
} 
double GetMinIntensity() 
{ 
return 0; 
return mMinIntensity; 
}27 
bool InitPOIIndexTree() 
{ 
std::string baseDir = FileSystem::ParentPath(mHeatmapParam.FilePath); 
std::string file = mHeatmapParam.HeatMapPOIFile; 
if (!FileSystem::FileExists(file)) 
file = FileSystem::CombinePath(baseDir, mHeatmapParam.HeatMapPOIFile); 
if (!FileSystem::FileExists(file)) 
{ 
LogIns.error("heatmap setting file not exist!{0}", mHeatmapParam.HeatMapPOIFile); 
return false; 
} 
csv::io::CSVReader<4> in(file); 
in.read_header(csv::io::ignore_extra_column, mHeatmapParam.LongitudeField, 
mHeatmapParam.LatitudeField, mHeatmapParam.HeightField, mHeatmapParam.IntensityField); 
double l, b, h, i; 
while (in.read_row(l, b, h, i)) { 
POIPtr p1 = std::make_shared<POI>(); 
p1->X() = l; 
p1->Y() = b; 
p1->Z() = h; 
p1->Intensity = i; 
if (mMaxIntensity < i) 
mMaxIntensity = i; 
if (mMinIntensity > i) 
mMinIntensity = i; 
mPtrTransformation->Transform(p1->X(), p1->Y(), p1->Z()); 
mPOITree.Insert(p1->LBH, p1->LBH, p1); 
} 
return true; 
} 
protected: 
std::vector<POIPtr> Search(double x, double y, double z, double distance) 
{ 
std::vector<double> min{ x - distance,y - distance,z - distance }; 
std::vector<double> max{ x + distance,y + distance,z + distance }; 
double distance2 = distance * distance; 
std::vector<POIPtr> res; 
mPOITree.Search(min.data(), max.data(), [&res, x, y, z, distance2](const POIPtr& ptr) { 
double xx = std::fabs(ptr->X() - x); 
double yy = std::fabs(ptr->Y() - y); 
double zz = std::fabs(ptr->Z() - z); 
double dd = xx * xx + yy * yy + zz * zz; 
if (dd > distance2) 
return true; 
res.push_back(ptr); 
return true; 
});28 
return res; 
} 
double Distance(const Point3d& p1, const Point3d& p2) 
{ 
double a = std::fabs(p1.x() - p2.x()); 
double b = std::fabs(p1.y() - p2.y()); 
double c = std::fabs(p1.z() - p2.z()); 
return sqrt(a * a + b * b + c * c); 
} 
int CalcuteHeightSize(const BoundingVolumeRegion& region) 
{ 
Point3d p1(region.Data[0], region.Data[1], region.Data[4]); 
Point3d p2(region.Data[2], region.Data[1], region.Data[4]); 
Point3d p3(region.Data[0], region.Data[3], region.Data[4]); 
mPtrTransformation->Transform(p1.x(), p1.y(), p1.z()); 
mPtrTransformation->Transform(p2.x(), p2.y(), p2.z()); 
mPtrTransformation->Transform(p3.x(), p3.y(), p3.z()); 
double width = Distance(p1, p2); 
double height = Distance(p1, p3); 
double depth = region.Depth(); 
int n = mTextureSize / std::max(width, height) * depth; 
int nHeight = 2; 
for (int i = 1; i < 6; ++i) 
{ 
nHeight = 2 << i; 
if (n < nHeight) 
return nHeight; 
} 
return 64; 
} 
void CalcuteHeightSize(const BoundingVolumeRegion& region, int& height_w, int& height_h) 
{ 
Point3d p1(region.Data[0], region.Data[1], region.Data[4]); 
Point3d p2(region.Data[2], region.Data[1], region.Data[4]); 
Point3d p3(region.Data[0], region.Data[3], region.Data[4]); 
mPtrTransformation->Transform(p1.x(), p1.y(), p1.z()); 
mPtrTransformation->Transform(p2.x(), p2.y(), p2.z()); 
mPtrTransformation->Transform(p3.x(), p3.y(), p3.z()); 
double width = Distance(p1, p2); 
double height = Distance(p1, p3); 
double depth = region.Depth();29 
int n = mTextureSize / std::max(width, height) * depth; 
int nHeight = 2; 
for (int i = 1; i < 3; ++i) 
{ 
nHeight = 2 << i; 
if (n < nHeight * nHeight) 
{ 
break; 
} 
} 
//height_w = nHeight * 2; 
//height_h = nHeight / 2; 
height_w = nHeight * nHeight; 
height_h = 1; 
} 
protected: 
RTree<POIPtr, double, 3> mPOITree; 
TransformationPtr mPtrTransformation; 
double mMaxIntensity; 
double mMinIntensity; 
HeatmapParam mHeatmapParam; 
std::function<double(const std::vector<POIPtr>&, const Point3d&)> mHeatmapAlgo; 
int mTextureSize; 
}; 
using HeatMapPOIPtr = std::shared_ptr<HeatMapPOI>; 
class KHR_techniques_webgl_Extension 
{ 
public: 
static void Add(tinygltf::Model& model, double xMin, double xMax, double yMin, double yMax, double zMin, 
double zMax, 
int w, int h, int len, double max, double min, double totalMax, double totalMin) 
{ 
tinygltf::Value::Object root; 
tinygltf::Value::Array programs; 
{ 
tinygltf::Value::Object item; 
item["fragmentShader"] = tinygltf::Value{ 0 }; 
item["vertexShader"] = tinygltf::Value{ 1 }; 
programs.emplace_back(item); 
} 
tinygltf::Value::Array shaders; 
{ 
{ 
tinygltf::Value::Object item;30 
item["type"] = tinygltf::Value{ 35632 }; 
item["uri"] = tinygltf::Value{ std::string("fs.glsl") }; 
shaders.emplace_back(item); 
}
{ 
tinygltf::Value::Object item; 
item["type"] = tinygltf::Value{ 35633 }; 
item["uri"] = tinygltf::Value{ std::string("vs.glsl") }; 
shaders.emplace_back(item); 
} 
} 
tinygltf::Value::Array techniques; 
{ 
tinygltf::Value::Object item; 
item["program"] = tinygltf::Value{ 0 }; 
{ 
tinygltf::Value::Object attributes; 
{ 
tinygltf::Value::Object att_normal; 
att_normal["semantic"] = tinygltf::Value{ std::string("NORMAL") }; 
att_normal["type"] = tinygltf::Value{ 35665 }; 
attributes["a_normal"] = tinygltf::Value{ std::move(att_normal) }; 
tinygltf::Value::Object att_position; 
att_position["semantic"] = tinygltf::Value{ std::string("POSITION") }; 
att_position["type"] = tinygltf::Value{ 35665 }; 
attributes["a_position"] = tinygltf::Value{ std::move(att_position) }; 
tinygltf::Value::Object att_texcoord0; 
att_texcoord0["semantic"] = tinygltf::Value{ std::string("TEXCOORD_0") }; 
att_texcoord0["type"] = tinygltf::Value{ 35664 }; 
attributes["a_texcoord0"] = tinygltf::Value{ std::move(att_texcoord0) }; 
tinygltf::Value::Object att_batchid; 
att_batchid["semantic"] = tinygltf::Value{ std::string("_BATCHID") }; 
att_batchid["type"] = tinygltf::Value{ 5123 }; 
attributes["a_batchId"] = tinygltf::Value{ std::move(att_batchid) }; 
} 
item["attributes"] = tinygltf::Value{ attributes }; 
}
{ 
tinygltf::Value::Object uniforms; 
{ 
tinygltf::Value::Object width; 
width["type"] = tinygltf::Value{ 5126 }; 
width["value"] = tinygltf::Value{ w };31 
uniforms["width"] = tinygltf::Value{ std::move(width) }; 
tinygltf::Value::Object height; 
height["type"] = tinygltf::Value{ 5126 }; 
height["value"] = tinygltf::Value{ h }; 
uniforms["height"] = tinygltf::Value{ std::move(height) }; 
tinygltf::Value::Object depth; 
depth["type"] = tinygltf::Value{ 5126 }; 
depth["value"] = tinygltf::Value{ len }; 
uniforms["depth"] = tinygltf::Value{ std::move(depth) }; 
tinygltf::Value::Object total_max; 
total_max["type"] = tinygltf::Value{ 5126 }; 
total_max["value"] = tinygltf::Value{ totalMax }; 
uniforms["total_max"] = tinygltf::Value{ std::move(total_max) }; 
tinygltf::Value::Object total_min; 
total_min["type"] = tinygltf::Value{ 5126 }; 
total_min["value"] = tinygltf::Value{ totalMin }; 
uniforms["total_min"] = tinygltf::Value{ std::move(total_min) }; 
tinygltf::Value::Object i_max; 
i_max["type"] = tinygltf::Value{ 5126 }; 
i_max["value"] = tinygltf::Value{ max }; 
uniforms["i_max"] = tinygltf::Value{ std::move(i_max) }; 
tinygltf::Value::Object i_min; 
i_min["type"] = tinygltf::Value{ 5126 }; 
i_min["value"] = tinygltf::Value{ min }; 
uniforms["i_min"] = tinygltf::Value{ std::move(i_min) }; 
tinygltf::Value::Object x_min; 
x_min["type"] = tinygltf::Value{ 5126 }; 
x_min["value"] = tinygltf::Value{ xMin }; 
uniforms["x_min"] = tinygltf::Value{ std::move(x_min) }; 
tinygltf::Value::Object x_max; 
x_max["type"] = tinygltf::Value{ 5126 }; 
x_max["value"] = tinygltf::Value{ xMax }; 
uniforms["x_max"] = tinygltf::Value{ std::move(x_max) }; 
tinygltf::Value::Object y_min; 
y_min["type"] = tinygltf::Value{ 5126 }; 
y_min["value"] = tinygltf::Value{ yMin }; 
uniforms["y_min"] = tinygltf::Value{ std::move(y_min) }; 
tinygltf::Value::Object y_max; 
y_max["type"] = tinygltf::Value{ 5126 };32 
y_max["value"] = tinygltf::Value{ yMax }; 
uniforms["y_max"] = tinygltf::Value{ std::move(y_max) }; 
tinygltf::Value::Object z_min; 
z_min["type"] = tinygltf::Value{ 5126 }; 
z_min["value"] = tinygltf::Value{ zMin }; 
uniforms["z_min"] = tinygltf::Value{ std::move(z_min) }; 
tinygltf::Value::Object z_max; 
z_max["type"] = tinygltf::Value{ 5126 }; 
z_max["value"] = tinygltf::Value{ zMax }; 
uniforms["z_max"] = tinygltf::Value{ std::move(z_max) }; 
tinygltf::Value::Object z_size; 
z_max["type"] = tinygltf::Value{ 5126 }; 
z_max["value"] = tinygltf::Value{ len }; 
uniforms["z_size"] = tinygltf::Value{ std::move(z_max) }; 
tinygltf::Value::Object u_ambient; 
u_ambient["type"] = tinygltf::Value{ 35666 }; 
uniforms["u_ambient"] = tinygltf::Value{ std::move(u_ambient) }; 
tinygltf::Value::Object u_diffuse; 
u_diffuse["type"] = tinygltf::Value{ 35666 }; 
uniforms["u_diffuse"] = tinygltf::Value{ std::move(u_diffuse) }; 
tinygltf::Value::Object u_light; 
u_light["type"] = tinygltf::Value{ 5126 }; 
uniforms["u_light"] = tinygltf::Value{ std::move(u_light) }; 
tinygltf::Value::Object u_modelViewMatrix; 
u_modelViewMatrix["type"] = tinygltf::Value{ 35676 }; 
u_modelViewMatrix["semantic"] = tinygltf::Value{ std::string("MODELVIEW") }; 
uniforms["u_modelViewMatrix"] = tinygltf::Value{ std::move(u_modelViewMatrix) }; 
tinygltf::Value::Object u_normalMatrix; 
u_normalMatrix["type"] = tinygltf::Value{ 35675 }; 
u_normalMatrix["semantic"] = 
tinygltf::Value{ std::string("MODELVIEWINVERSETRANSPOSE") }; 
uniforms["u_normalMatrix"] = tinygltf::Value{ std::move(u_normalMatrix) }; 
tinygltf::Value::Object u_projectionMatrix; 
u_projectionMatrix["type"] = tinygltf::Value{ 35676 }; 
u_projectionMatrix["semantic"] = tinygltf::Value{ std::string("PROJECTION") }; 
uniforms["u_projectionMatrix"] = tinygltf::Value{ std::move(u_projectionMatrix) }; 
tinygltf::Value::Object u_shininess; 
u_shininess["type"] = tinygltf::Value{ 5126 }; 
uniforms["u_shininess"] = tinygltf::Value{ std::move(u_shininess) };33 
tinygltf::Value::Object u_specular; 
u_specular["type"] = tinygltf::Value{ 35666 }; 
uniforms["u_specular"] = tinygltf::Value{ std::move(u_specular) }; 
tinygltf::Value::Object u_texImage; 
u_texImage["type"] = tinygltf::Value{ 35678 }; 
uniforms["u_texImage"] = tinygltf::Value{ std::move(u_texImage) }; 
} 
item["uniforms"] = tinygltf::Value{ std::move(uniforms) }; 
} 
techniques.emplace_back(item); 
} 
tinygltf::Value::Object content; 
content["programs"] = tinygltf::Value{ std::move(programs) }; 
content["shaders"] = tinygltf::Value{ std::move(shaders) }; 
content["techniques"] = tinygltf::Value{ std::move(techniques) }; 
model.extensions["KHR_techniques_webgl"] = tinygltf::Value{ std::move(content) }; 
model.extensionsUsed.push_back("KHR_techniques_webgl"); 
model.extensionsRequired.push_back("KHR_techniques_webgl"); 
tinygltf::Value::Object materialExtenContent; 
materialExtenContent["technique"] = tinygltf::Value{ 0 }; 
{ 
tinygltf::Value::Object valuse; 
{ 
tinygltf::Value::Array u_ambient; 
u_ambient.emplace_back(0.541176); 
u_ambient.emplace_back(0.584314); 
u_ambient.emplace_back(0.5921569999999999); 
u_ambient.emplace_back(1); 
valuse["u_ambient"] = tinygltf::Value{ std::move(u_ambient) }; 
}
{ 
tinygltf::Value::Array u_diffuse; 
u_diffuse.emplace_back(0.541176); 
u_diffuse.emplace_back(0.584314); 
u_diffuse.emplace_back(0.5921569999999999); 
u_diffuse.emplace_back(1); 
valuse["u_diffuse"] = tinygltf::Value{ std::move(u_diffuse) }; 
}
{ 
tinygltf::Value::Array u_emissive; 
u_emissive.emplace_back(0);34 
u_emissive.emplace_back(0); 
u_emissive.emplace_back(0); 
u_emissive.emplace_back(1); 
valuse["u_emissive"] = tinygltf::Value{ std::move(u_emissive) }; 
} 
valuse["u_light"] = tinygltf::Value{ 1 }; 
valuse["u_shinniness"] = tinygltf::Value{ 0 }; 
{ 
tinygltf::Value::Array u_specular; 
u_specular.emplace_back(0); 
u_specular.emplace_back(0); 
u_specular.emplace_back(0); 
u_specular.emplace_back(1); 
valuse["u_specular"] = tinygltf::Value{ std::move(u_specular) }; 
}
{ 
tinygltf::Value::Object u_texImage; 
u_texImage["index"] = tinygltf::Value{ 0 }; 
u_texImage["texCoord"] = tinygltf::Value{ 0 }; 
valuse["u_texImage"] = tinygltf::Value{ std::move(u_texImage) }; 
} 
materialExtenContent["values"] = tinygltf::Value{ std::move(valuse) }; 
} 
for (auto& mt : model.materials) 
{ 
mt.extensions["KHR_techniques_webgl"] = tinygltf::Value{ std::move(materialExtenContent) }; 
} 
} 
}; 
class ShapeConvert 
{ 
public: 
ShapeConvert(const Shp2ModelParam& param, const ModelBuildArgs& buildArgs) 
:mParam(param), mTotalCount(0), mStep(0), mCurrentCount(0), mFeaCurCount(0), 
mBuildArgs(buildArgs) 
{ 
mPtrFeaCache = std::make_shared<FeatureCache>(mParam.ShapeFilePath); 
mPtrDefaultTexture = std::make_shared<BuildingTexture>(); 
mPtrDefaultTexture->Param.WallTexture.Width = 1; 
mPtrDefaultTexture->Param.WallTexture.Height = 1; 
mPtrDefaultTexture->Param.RoofTexture.Width = 1; 
mPtrDefaultTexture->Param.RoofTexture.Height = 1; 
for (auto& buildingTextureParam : mParam.BuildingTextures) 
{ 
auto ptrBuildingTexture = std::make_shared<BuildingTexture>(); 
ptrBuildingTexture->Param = buildingTextureParam; 
std::string baseDir = FileSystem::ParentPath(buildingTextureParam.FilePath);35 
std::string wallImg = buildingTextureParam.WallTexture.ImgFilePath; 
if(!FileSystem::FileExists(wallImg)) 
wallImg = FileSystem::CombinePath(baseDir, 
buildingTextureParam.WallTexture.ImgFilePath); 
if (!FileSystem::FileExists(wallImg)) 
{ 
LogIns.error("{0} wall texture not exist!", buildingTextureParam.WallTexture.ImgFilePath); 
} 
else 
{ 
auto ptrWall = std::make_shared<Texture>(); 
ptrWall->SetFilePath(wallImg); 
ptrWall->SetName("wall_texture"); 
ptrBuildingTexture->PtrWallTexture = ptrWall; 
} 
std::string roofImg = buildingTextureParam.RoofTexture.ImgFilePath; 
if (!FileSystem::FileExists(roofImg)) 
roofImg = FileSystem::CombinePath(baseDir, 
buildingTextureParam.RoofTexture.ImgFilePath); 
if (!FileSystem::FileExists(roofImg)){ 
LogIns.error("{0} roof texture not exist!", buildingTextureParam.RoofTexture.ImgFilePath); 
} 
else 
{ 
auto ptrRoof = std::make_shared<Texture>(); 
ptrRoof->SetFilePath(roofImg); 
ptrRoof->SetName("roof_texture"); 
ptrBuildingTexture->PtrRoofTexture = ptrRoof; 
} 
mVecBuildingTexture.push_back(ptrBuildingTexture); 
} 
if (!mParam.heatmapParam.HeatMapPOIFile.empty()) 
{ 
mPtrHeatMap = HeatMapPOI::Create(mParam.heatmapParam); 
if (mPtrHeatMap == nullptr) 
mIsValid = false; 
} 
} 
bool Process() 
{ 
if (!mIsValid) 
return false; 
const auto& totalRegion = mPtrFeaCache->GetRegion(); 
mTotalCount = 0; 
mFeaCurCount = 0; 
// 标准金字塔 
auto* pyramid = Pyramid::instance(); 
//计算行列号36 
unsigned int MinRow, MinCol, MaxRow, MaxCol; 
for (int i = mParam.StartLevel; i <= mParam.EndLevel; ++i) 
{ 
pyramid->getTile(i, Math::ToRadian(totalRegion.Data[0]), Math::ToRadian(totalRegion.Data[1]) 
, Math::ToRadian(totalRegion.Data[2]), Math::ToRadian(totalRegion.Data[3]), 
MinRow, MinCol, MaxRow, MaxCol); 
mTotalCount += (std::fabs((int)MaxRow - (int)MinRow) + 1) * (std::fabs((int)MaxCol - (int)MinCol) 
+ 1); 
auto ptrTask = std::make_shared<Shp2ModelTileByLevel>(i, MinRow, MinCol, MaxRow, MaxCol); 
ptrTask->MergeTaskFunc = std::bind(&ShapeConvert::MergeTileData, this, std::placeholders::_1, 
std::placeholders::_2); 
mMapMergeTaskLevel[i] = ptrTask; 
} 
mStep = mTotalCount / 200.0; 
if (mStep == 0) mStep = 1; 
int level = mParam.EndLevel; 
pyramid->getTile(level, Math::ToRadian(totalRegion.Data[0]), Math::ToRadian(totalRegion.Data[1]) 
, Math::ToRadian(totalRegion.Data[2]), Math::ToRadian(totalRegion.Data[3]), 
MinRow, MinCol, MaxRow, MaxCol); 
//调整地理范围 
if (MinRow % 2 != 0) MinRow--; 
if (MinCol % 2 != 0) MinCol--; 
//希尔伯特曲线遍历瓦片 
PyramidHCurveVisitor tileVisitor(MinRow, MinCol, MaxRow + 1, MaxCol + 1); 
unsigned int r, c; 
//auto totalCenter = totalRegion.Center3D(); 
while (tileVisitor.Next(r, c)) 
{ 
if (Core::GetSingletonPtr() != nullptr && Core::GetSingletonPtr()->IsStop()) 
return false; 
BoundingVolumeRegion dataRegion; 
BoundingVolumeRegion region; 
region.Data = std::vector<double>(6, 0); 
PyramidKey tileKey(level, r, c); 
pyramid->getTilePos(level, r, c, region.Data[0], region.Data[1], region.Data[2], region.Data[3]); 
//转换成度 
for (size_t i = 0; i < 4; ++i) 
{ 
region.Data[i] = Math::ToDegree(region.Data[i]); 
} 
auto features = mPtrFeaCache->SearchByCenter(region); 
if (features.empty()) 
{ 
auto ptrTask = this->MergeTask(tileKey.Level); 
ptrTask->Add(tileKey); 
IncreaseProgress();37 
continue; 
} 
mThreadPool.enqueue([this, features, tileKey]() { 
std::string heightField = this->mParam.HeightField; 
std::string filterFiled = this->mParam.FilterField; 
std::string floorField = this->mParam.FloorField; 
std::string floorHeightField = this->mParam.FloorHeightField; 
bool useFloor = false; 
if (heightField.empty() && !floorField.empty() && !floorHeightField.empty()) 
{ 
useFloor = true; 
} 
Shp2ModelTileDataPtr ptrTileData = std::make_shared<Shp2ModelTileData>(); 
ptrTileData->Key = tileKey; 
ptrTileData->Level = tileKey.Level; 
BoundingVolumeRegion geoRegion; 
std::shared_ptr<ThreadPool<>> ptrThreadPool; 
if (features.size() > 16) 
ptrThreadPool = std::make_shared<ThreadPool<>>(); 
std::mutex mtx; 
for (auto& fea : features) 
{ 
++mFeaCurCount; 
auto ptrFea = fea->GetFeaturePtr(); 
if (!ptrFea) 
continue; 
auto* pFea = ptrFea->GetRefFeature(); 
if (pFea == nullptr) 
continue; 
double height = this->mParam.DefaultHeight; 
if (height <= 0) 
height = 0.1; 
if (!heightField.empty() && pFea->GetFieldIndex(heightField.c_str())) 
height = pFea->GetFieldAsDouble(heightField.c_str()); 
BuildingTexturePtr ptrBuildTexture = mPtrDefaultTexture; 
if (!filterFiled.empty()) 
{ 
std::string filter = pFea->GetFieldAsString(filterFiled.c_str()); 
for (auto& buildingTexture : mVecBuildingTexture) 
{ 
if (filter == buildingTexture->Filter) 
ptrBuildTexture = buildingTexture; 
} 
} 
int floor = 0; 
double floorHeight = 0; 
if (useFloor) 
{ 
floor = pFea->GetFieldAsInteger(floorField.c_str());38 
floorHeight = pFea->GetFieldAsDouble(floorHeightField.c_str()); 
if (floorHeight <= 0) 
floorHeight = 3; 
} 
if(!mVecBuildingTexture.empty())//random 
{ 
int index = mFeaCurCount % mVecBuildingTexture.size(); 
ptrBuildTexture = mVecBuildingTexture[index]; 
} 
auto threadFun = [this,tileKey,ptrTileData, fea, &geoRegion, ptrFea, 
height, &mtx, ptrBuildTexture,useFloor,floor,floorHeight]() { 
TexturePtr ptrWallTexture = nullptr; 
TexturePtr ptrRoofTexture = nullptr; 
ModelPtr ptrModel; 
if (!useFloor) 
{ 
ptrModel = FeatureBuilder::Convert2Model(ptrFea, height, 
ptrBuildTexture); 
} 
else 
{ 
ptrModel = FeatureBuilder::Convert2ModelByFloor(ptrFea, floor, 
floorHeight, ptrBuildTexture); 
} 
if (ptrModel == nullptr) 
return; 
double area = ptrModel->GetArea(); 
{ 
std::lock_guard<std::mutex> lock(mtx); 
geoRegion.Union(ptrModel->GetGeoBoundingBox()); 
Shp2ModelModelData modelData; 
modelData.PtrModel = ptrModel; 
modelData.Area = area; 
modelData.pFeaIndex = fea; 
ptrTileData->Models.push_back(modelData); 
}}; 
//if (ptrThreadPool) 
// 
ptrThreadPool->enqueue(threadFun); 
//else 
threadFun(); 
} 
if (ptrThreadPool) 
{ 
ptrThreadPool->Stop(); 
ptrThreadPool.reset(); 
} 
auto ptrTask = this->MergeTask(tileKey.Level); 
if (ptrTileData->Models.empty()) 
{39 
ptrTask->Add(ptrTileData->Key); 
this->IncreaseProgress(); 
return; 
} 
else 
{ 
ptrTileData->Region = geoRegion; 
this->WriteTileData(ptrTileData); 
this->IncreaseProgress(); 
ptrTileData->Node.geometricError = 0; 
ptrTask->Add(ptrTileData); 
} 
}); 
} 
mThreadPool.Stop(); 
std::string outFile = FileSystem::CombinePath(mParam.OutputFilePath, "tileset.json"); 
outFile = StringUtil::ReplaceAllString(outFile, "\\", "/"); 
TilesetJson json; 
json.root.geometricError = CalculateGeometricError(mParam.StartLevel - 1); 
json.geometricError = json.root.geometricError; 
if (mPtrHeatMap) 
{ 
json.attributes["max_intensity"] = mPtrHeatMap->GetMaxIntensity(); 
json.attributes["min_intensity"] = mPtrHeatMap->GetMinIntensity(); 
json.attributes["is_heatmap"] = true; 
} 
BoundingVolumeRegion region; 
for (auto& data : mRootTileDatas) 
{ 
region.Union(data->Region); 
} 
auto ptrModelCS = std::make_shared<ModelCoordinateSystem>(region.Center3D()); 
for (auto& data : mRootTileDatas) 
{ 
data->Node.transform = ModelCoordinateSystem::ENURelativeMatrix(data->Region.Center3D(), 
region.Center3D()); 
json.root.children.push_back(data->Node); 
} 
ptrModelCS->Geo2Model(region); 
if (mBuildArgs.mEpGisConvertFlag == 1 && Core::GetSingletonPtr() && 
Core::GetSingletonPtr()->GetEpGisTransformation()) 
{ 
auto ptCenter = ptrModelCS->GetOrigin();40 
auto ptrCore = Core::GetSingletonPtr(); 
if (ptrCore == nullptr) 
return false; 
if (!ptrCore->GetEpGisTransformation()->ConvertEpGis(ptCenter.x(), ptCenter.y())) 
{ 
LogIns.error("ShapeConvert epgis transformation failed!"); 
} 
auto ptrTempModelCS = std::make_shared<ModelCoordinateSystem>(ptCenter); 
json.root.transform = ptrTempModelCS->GetTransformMatirx(); 
} 
else 
json.root.transform = ptrModelCS->GetTransformMatirx(); 
json.root.boundingVolume.Box = region; 
std::string jsonStr = json.ToString(mParam.OutputFilePath); 
mParam.PtrModelDataSet->Save(outFile, jsonStr, mParam.PtrModelDataSet->compressOpts); 
CreateMeta(mParam.OutputFilePath, mParam.PtrModelDataSet); 
if (mPtrHeatMap && mParam.heatmapParam.UserCustomerShader) 
{ 
std::string epgsdataDir = FileSystem::CombinePath(mParam.OutputFilePath, "epgis_data"); 
std::string vsFile = FileSystem::CombinePath(epgsdataDir, "vs.glsl"); 
std::string fsFile = FileSystem::CombinePath(epgsdataDir, "fs.glsl"); 
vsFile = StringUtil::ReplaceAllString(vsFile, "\\", "/"); 
fsFile = StringUtil::ReplaceAllString(fsFile, "\\", "/"); 
mParam.PtrModelDataSet->Save(vsFile, VERTEX_SHADER, 
mParam.PtrModelDataSet->compressOpts); 
mParam.PtrModelDataSet->Save(fsFile, FRAGMENT_SHADER, 
mParam.PtrModelDataSet->compressOpts); 
} 
IncreaseProgress(true); 
return true; 
} 
~ShapeConvert() 
{
} 
protected: 
ModelPtr Convert2Model(const std::vector<FeatureIndex*>& feas) 
{ 
ModelPtr ptrModel = std::make_shared<Model>(); 
return ptrModel; 
} 
Shp2ModelTileByLevelPtr MergeTask(int level) 
{41 
std::lock_guard<std::mutex> lock(mMutex); 
auto it = mMapMergeTaskLevel.find(level); 
if (it == mMapMergeTaskLevel.end()) 
return nullptr; 
return it->second; 
} 
double GetSimplifyParam(int level) 
{ 
return 0.5; 
} 
double CalculateGeometricError(unsigned int level) 
{ 
static double ScreenSpaceError = 16; 
static double EarthRadius = 6371393.0; 
static double pi = std::acos(-1); 
static double zerrorError = EarthRadius * pi / 2 / ScreenSpaceError; 
unsigned int base = (1 << (level - 1)); 
return zerrorError / base; 
} 
void WriteTileData(Shp2ModelTileDataPtr ptrTileData) 
{ 
std::string name = std::to_string(ptrTileData->Key.Level) + "_" 
+ std::to_string(ptrTileData->Key.Row) + "_" + std::to_string(ptrTileData->Key.Col) + ".eb3d"; 
std::string b3dmFileDir = FileSystem::CombinePath(mParam.OutputFilePath, "epgis_data"); 
std::string b3dmFile = FileSystem::CombinePath("epgis_data", name); 
ModelPtr ptrModel = std::make_shared<Model>(); 
auto center = ptrTileData->Region.Center3D(); 
ptrModel->SetLongitude(center.x()); 
ptrModel->SetLatitude(center.y()); 
ptrModel->SetHeight(center.z()); 
auto ptrTarModelCS = ptrModel->GetModelCSPtr(); 
TransformationProcessor processor(ptrModel, ptrTarModelCS); 
for (auto& data : ptrTileData->Models) 
{ 
processor.Process(data.PtrModel); 
} 
ptrModel->DirtyBound(); 
int w = 0, h = 0, len = 0; 
double min = 0, max = 0; 
if (mPtrHeatMap) 
{ 
mPtrHeatMap->Processor(ptrModel, w, h, len, max, min); 
} 
MergeMeshByTextureModelProcessor mergeProcessor; 
mergeProcessor.Process(ptrModel);42 
ConvertTextureToDDSProcessor ddsProcessor; 
//ddsProcessor.Process(ptrModel); 
Tile3DData::Batched3DModel tempB3dm; 
DracoCompression::EncodeSetting encodeSetting; 
encodeSetting.compressionLevel = -1; 
tempB3dm.SetDracoEncodeSetting(encodeSetting); 
ptrModel->ExportB3dmStreamWithEmbeddedTextures(tempB3dm); 
std::string outFilePath = FileSystem::CombinePath(mParam.OutputFilePath, b3dmFile); 
outFilePath = StringUtil::ReplaceAllString(outFilePath, "\\", "/"); 
std::set<int> ids; 
for (auto& mesh : ptrModel->GetMeshArray()) 
{ 
auto ptrBatchID = mesh->GetBatchIDArrayPtr(); 
for (auto& id : *ptrBatchID) 
{ 
ids.insert(id); 
} 
} 
nlohmann::json featureTable; 
if (mPtrHeatMap) 
{ 
featureTable["min_intensity"] = min; 
featureTable["max_intensity"] = max; 
featureTable["height_w"] = w; 
featureTable["height_h"] = h; 
} 
featureTable["BATCH_LENGTH"] = ids.size(); 
tempB3dm.SetFeatureTableJson(featureTable.dump()); 
nlohmann::json batchTable; 
batchTable["batchid"] = ids; 
auto& atts = ptrModel->GetAttributes(); 
{ 
for (auto& item : atts) 
{ 
nlohmann::json values; 
for (auto& v : item.second) 
values.push_back(v.second); 
batchTable[item.first] = values; 
} 
} 
std::string tempStr = batchTable.dump(); 
tempB3dm.SetBatchTableJson(batchTable.dump()); 
if (mPtrHeatMap && mParam.heatmapParam.UserCustomerShader)43 
{ 
//gltf 
std::string glb; 
tempB3dm.GetGlb(glb); 
tinygltf::TinyGLTF tlgf; 
tinygltf::Model model; 
std::string errMsg, warnMsg; 
tlgf.LoadBinaryFromMemory(&model, &errMsg, &warnMsg, (const unsigned char*)glb.data(), 
glb.size()); 
auto ptrModelCS = ptrModel->GetModelCSPtr(); 
auto geoBound = ptrModel->GetGeoBoundingBox(); 
ptrModelCS->Geo2Model(geoBound); 
KHR_techniques_webgl_Extension::Add(model, geoBound.Data[0], geoBound.Data[2], 
geoBound.Data[1], 
geoBound.Data[3], geoBound.Data[4], geoBound.Data[5], w, h, len, max, min, 
mPtrHeatMap->GetMaxIntensity(), mPtrHeatMap->GetMinIntensity()); 
std::ostringstream ostr; 
tlgf.WriteGltfSceneToStream(&model, ostr, false, true); 
std::string oglb = ostr.str(); 
tempB3dm.SetGlb(oglb); 
} 
string ss; 
tempB3dm.Export(ss); 
if (!ss.empty()) 
mParam.PtrModelDataSet->Save(outFilePath, ss, mParam.PtrModelDataSet->compressOpts); 
ptrTileData->Node.content.uri = b3dmFile; 
auto region = ptrTileData->Region; 
ptrTarModelCS->Geo2Model(region); 
ptrTileData->Node.boundingVolume.Box = region; 
ptrTileData->Node.geometricError = CalculateGeometricError(ptrTileData->Key.Level); 
ptrTileData->Node.refine = RefineType::eReplace; 
} 
bool MergeTileData(const std::vector<Shp2ModelTileDataPtr>& vecNode, const PyramidKey& key) 
{ 
if ((int)key.Level < mParam.StartLevel) 
{ 
for (auto& data : vecNode) 
{ 
std::lock_guard<std::mutex> lock(mRootMtx); 
mRootTileDatas.push_back(data); 
} 
return true; 
}44 
if (vecNode.empty()) 
{ 
auto ptrTask = this->MergeTask(key.Level); 
ptrTask->Add(key); 
return true; 
} 
Shp2ModelTileDataPtr ptrTileData = std::make_shared<Shp2ModelTileData>(); 
ptrTileData->Key = key; 
ptrTileData->Level = key.Level; 
double threshold = GetSimplifyParam(key.Level); 
for (auto& data : vecNode) 
{ 
ptrTileData->Region.Union(data->Region); 
std::copy(data->Models.begin(), data->Models.end(), std::back_inserter(ptrTileData->Models)); 
} 
auto ptrTask = this->MergeTask(key.Level); 
if (ptrTileData->Models.empty()) 
{ 
ptrTask->Add(key); 
this->IncreaseProgress(); 
return true; 
} 
std::sort(ptrTileData->Models.begin(), ptrTileData->Models.end(), 
[](const Shp2ModelModelData& left, const Shp2ModelModelData& right) { 
return left.Area > right.Area; 
}); 
int step = threshold * ptrTileData->Models.size() + 1; 
auto it = ptrTileData->Models.begin(); 
std::advance(it, step); 
if (it != ptrTileData->Models.end()) 
ptrTileData->Models.erase(it, ptrTileData->Models.end()); 
ptrTileData->Node.children.reserve(vecNode.size()); 
for (auto& data : vecNode) 
{ 
data->Node.transform = ModelCoordinateSystem::ENURelativeMatrix(data->Region.Center3D(), 
ptrTileData->Region.Center3D()); 
ptrTileData->Node.children.push_back(data->Node); 
} 
WriteTileData(ptrTileData); 
this->IncreaseProgress(); 
ptrTask->Add(ptrTileData); 
return true; 
} 
void IncreaseProgress(bool done = false) 
{ 
if (done)45 
{ 
LogIns.debug("Progress: {0}%", 100); 
return; 
} 
++mCurrentCount; 
if (mCurrentCount % mStep == 0 && Core::GetSingletonPtr()) 
{ 
auto ptrProgress = Core::GetSingletonPtr()->GetModelProduceProgress(); 
if (ptrProgress) 
ptrProgress->NotifyProgress("produce model", mCurrentCount, mTotalCount, "produce 
model"); 
float progress = mCurrentCount * 100.0 / mTotalCount; 
if (progress >= 100) 
progress = 99.99; 
LogIns.debug("Progress: {0}%", progress); 
} 
} 
//meta.json 文件 
bool CreateMeta(const std::string& baseDir, ModelDataSetPtr mPtrModelDataSet) 
{ 
nlohmann::json json; 
json["type"] = "Model"; 
bool useGzip = false; 
if (mPtrModelDataSet) { 
useGzip = (mPtrModelDataSet->compressOpts == CompressOptions::CO_GZIP); 
} 
json["ziped"] = useGzip; 
std::string finalStr = json.dump(); 
std::string metaJsonFileName = Config::GetValue_String("META_JSON_FILENAME", 
META_JSON_FILENAME); 
metaJsonFileName = FileSystem::CombinePath(baseDir, metaJsonFileName); 
StringUtil::ReplaceAllString(metaJsonFileName, "\\", "/"); 
bool ret = false; 
if (mPtrModelDataSet) { 
ret = mPtrModelDataSet->Save(metaJsonFileName, finalStr, CompressOptions::CO_NONE); 
} 
else { 
ret = FileSystem::WriteFile(metaJsonFileName.c_str(), finalStr.data(), finalStr.size()); 
} 
return ret; 
} 
protected: 
Shp2ModelParam mParam; 
ModelBuildArgs mBuildArgs; 
BuildingTexturePtr mPtrDefaultTexture; 
std::vector<BuildingTexturePtr> mVecBuildingTexture; 
FeatureCachePtr mPtrFeaCache;46 
bool mIsValid = true; 
ThreadPool<> mThreadPool; 
std::mutex mMutex; 
std::map<int, Shp2ModelTileByLevelPtr> mMapMergeTaskLevel; 
TilesetNode mRoot; 
std::mutex mRootMtx; 
std::vector< Shp2ModelTileDataPtr> mRootTileDatas; 
size_t mTotalCount; 
size_t mStep; 
std::atomic<size_t> mCurrentCount; 
std::atomic<size_t> mFeaCurCount; 
HeatMapPOIPtr mPtrHeatMap; 
}; 
bool EPGIS_3D::Shp2Model::Convert(const Shp2ModelParam& param, const ModelBuildArgs& buildArgs) 
{ 
FileModelDataSet* pTileData = dynamic_cast<FileModelDataSet*>(param.PtrModelDataSet.get()); 
if (pTileData != nullptr) 
{ 
if (!FileSystem::DirExists(param.OutputFilePath)) 
FileSystem::CreateDir(param.OutputFilePath); 
std::string dataDir = FileSystem::CombinePath(param.OutputFilePath, "epgis_data"); 
if (!FileSystem::DirExists(dataDir)) 
FileSystem::CreateDir(dataDir); 
} 
ShapeConvert converter(param,buildArgs); 
return converter.Process(); 
} 
bool EPGIS_3D::Shp2ModelParam::ParseSetting(const std::string& jsonFile, Shp2ModelParam& param) 
{ 
if (jsonFile.empty() || !FileSystem::FileExists(jsonFile)) 
{ 
LogIns.error("Shp2ModelParam::ParseSetting failed! file isn't exist!{0}", jsonFile); 
return false; 
} 
std::ifstream in(jsonFile); 
if (!in.is_open()) 
{ 
LogIns.error("Shp2ModelParam::ParseSetting failed! can't open file {0}", jsonFile); 
return false; 
}47 
string jsonStr{ std::istreambuf_iterator<char>(in),std::istreambuf_iterator<char>() }; 
nlohmann::json textureJson; 
try 
{ 
textureJson = nlohmann::json::parse(jsonStr); 
} 
catch (const std::exception& e) 
{ 
LogIns.error("Parse setting file failed! file:{0},error {1}", jsonFile,e.what()); 
return false; 
} 
if (textureJson.find("HeightSetting") == textureJson.end()) 
{ 
LogIns.error("white model read configure failed! can't find HeightSetting filed! file:{0}", jsonFile); 
return false; 
} 
param.SettingFilePath = jsonFile; 
nlohmann::json& heightJson = textureJson["HeightSetting"]; 
try 
{ 
if (heightJson.find("DefaultField") != heightJson.end()) 
param.DefaultHeight = heightJson["DefaultField"].get<double>(); 
if (heightJson.find("HeightField") != heightJson.end()) 
param.HeightField = heightJson["HeightField"].get<std::string>(); 
if (heightJson.find("FloorField") != heightJson.end()) 
param.FloorField = heightJson["FloorField"].get<std::string>(); 
if (heightJson.find("FloorHeightField") != heightJson.end()) 
param.FloorHeightField = heightJson["FloorHeightField"].get<std::string>(); 
if (textureJson.find("BuildingTextureSetting") != textureJson.end()) 
ParseBuildingTextureSetting(textureJson["BuildingTextureSetting"], param); 
if (textureJson.find("HeatmapSetting") != textureJson.end()) 
ParseHeatmapSetting(textureJson["HeatmapSetting"], param); 
return true; 
} 
catch (const std::exception& e) 
{ 
LogIns.error("Parse setting file failed! file:{0},error {1}", jsonFile, e.what()); 
return false; 
} 
} 
void EPGIS_3D::Shp2ModelParam::ParseHeatmapSetting(const nlohmann::json& json, Shp2ModelParam& param) 
{ 
if (json.find("POIFile") == json.end()) 
{48 
LogIns.error("ParseHeatmapSetting failed! cant't find POIFile field!"); 
return; 
} 
param.heatmapParam.FilePath = param.SettingFilePath; 
param.heatmapParam.HeatMapPOIFile = json["POIFile"].get<std::string>(); 
if (json.find("LongitudeField") != json.end()) 
{ 
param.heatmapParam.LongitudeField = json["LongitudeField"].get<std::string>(); 
} 
if (json.find("LatitudeField") != json.end()) 
{ 
param.heatmapParam.LatitudeField = json["LatitudeField"].get<std::string>(); 
} 
if (json.find("HeightField") != json.end()) 
{ 
param.heatmapParam.HeightField = json["HeightField"].get<std::string>(); 
} 
if (json.find("IntensityField") != json.end()) 
{ 
param.heatmapParam.IntensityField = json["IntensityField"].get<std::string>(); 
} 
if (json.find("HeatmapAlgorithm") != json.end()) 
{ 
param.heatmapParam.HeatmapAlgorithm = json["HeatmapAlgorithm"].get<std::string>(); 
} 
if (json.find("MaxIntensity") != json.end()) 
{ 
param.heatmapParam.MaxIntensity = json["MaxIntensity"].get<double>(); 
} 
if (json.find("MinIntensity") != json.end()) 
{ 
param.heatmapParam.MinIntensity = json["MinIntensity"].get<double>(); 
} 
if (json.find("POIRadius") != json.end()) 
{ 
param.heatmapParam.POIRadius = json["POIRadius"].get<double>(); 
} 
if (json.find("HeatmapBlur") != json.end()) 
{ 
param.heatmapParam.HeatmapBlur = json["HeatmapBlur"].get<double>(); 
} 
if (json.find("UserCustomerShader") != json.end()) 
{ 
param.heatmapParam.UserCustomerShader = json["UserCustomerShader"].get<bool>(); 
} 
} 
void EPGIS_3D::Shp2ModelParam::ParseBuildingTextureSetting(const nlohmann::json& json, Shp2ModelParam& 
param) 
{49 
if (json.find("FilterField") != json.end()) 
{ 
param.FilterField = json["FilterField"].get<std::string>(); 
} 
if (json.find("BuildingTextures") != json.end()) 
{ 
for (auto& item : json["BuildingTextures"]) 
{ 
BuildingTextureParam txtParam; 
txtParam.FilePath = param.SettingFilePath; 
if (item.find("Filter") != item.end()) 
txtParam.Filter = item["Filter"].get<std::string>(); 
txtParam.WallTexture.ImgFilePath = 
StringConverter::UTF8ToANSI(item["WallTexture"]["Path"].get<std::string>()); 
txtParam.WallTexture.Width = item["WallTexture"]["Width"].get<double>(); 
txtParam.WallTexture.Height = item["WallTexture"]["Height"].get<double>(); 
txtParam.RoofTexture.ImgFilePath = 
StringConverter::UTF8ToANSI(item["RoofTexture"]["Path"].get<std::string>()); 
txtParam.RoofTexture.Width = item["RoofTexture"]["Width"].get<double>(); 
txtParam.RoofTexture.Height = item["RoofTexture"]["Height"].get<double>(); 
param.BuildingTextures.push_back(txtParam); 
} 
} 
} 
import defined from "../Core/defined.js"; 
import defaultValue from "../Core/defaultValue.js"; 
import Color from "../Core/Color.js"; 
import Texture from "../Renderer/Texture.js"; 
import PixelFormat from "../Core/PixelFormat.js"; 
function CreateRibbon(maxDensity, minDensity, colorGrid) { 
this._maxDensity = maxDensity; 
this._minDensity = minDensity; 
this._range = this._maxDensity - this._minDensity; 
this._colorGrid = colorGrid; 
this.rabbonTexture = undefined; 
this.rabbonSrc = new Uint8Array(256 * 4); 
this.needRefresh = true; 
} 
CreateRibbon.defaultColorGrid = [ 
{ 
intensity: 0, 
color: Color.fromCssColorString("#0000ff"), 
}, 
{ 
intensity: 10, 
color: Color.fromCssColorString("#00ff00"),}, 
{ 
intensity: 50, 
color: Color.fromCssColorString("#ffff00"), 
}, 
{ 
intensity: 100, 
color: Color.fromCssColorString("#ff0000"), 
}, 
]; 
Object.defineProperties(CreateRibbon.prototype, { 
maxDensity: { 
get: function () { 
return this._maxDensity; 
}, 
}, 
minDensity: { 
get: function () { 
return this._minDensity; 
}, 
}, 
}); 
CreateRibbon.prototype.resetColorGrid = function (colorGrid) { 
this.needRefresh = true; 
this._colorGrid = colorGrid; 
}; 
CreateRibbon.prototype.getColor = function (index) { 
var intensity = (this._range * index) / 256 + this._minDensity; 
var resultColor = new Color(); 
for (var i = 0; i < this._colorGrid.length; i++) { 
if (this._colorGrid[i].intensity >= intensity) { 
if (i === 0) { 
resultColor = this._colorGrid[i].color.clone(resultColor); 
return resultColor; 
} else { 
var interpolate = 
(intensity - this._colorGrid[i - 1].intensity) / 
(this._colorGrid[i].intensity - this._colorGrid[i - 1].intensity); 
resultColor = Color.lerp( 
this._colorGrid[i - 1].color, 
this._colorGrid[i].color, 
interpolate, 
resultColor 
); 
return resultColor; 
} 
} 
} 
}; 
50CreateRibbon.prototype.createBuffer = function () { 
var imagesource = this.rabbonSrc; 
for (var i = 0; i < 256; i++) { 
var color = this.getColor(i); 
imagesource[i * 4] = parseInt(color.red * 255); 
imagesource[i * 4 + 1] = parseInt(color.green * 255); 
imagesource[i * 4 + 2] = parseInt(color.blue * 255); 
imagesource[i * 4 + 3] = 255; 
} 
}; 
CreateRibbon.prototype.createTexture = function (context) { 
if (!this.needRefresh && defined(this.rabbonTexture)) { 
return this.rabbonTexture; 
} 
if (this.needRefresh) { 
this.createBuffer(); 
this.needRefresh = false; 
} 
var image = new ImageData(new Uint8ClampedArray(this.rabbonSrc), 1, 256); 
this.rabbonTexture = new Texture({ 
context: context, 
source: { 
arrayBufferView: image.data, 
}, 
width: 1, 
height: 256, 
pixelFormat: PixelFormat.RGBA, 
}); 
return this.rabbonTexture; 
}; 
CreateRibbon.prototype.destroy = function () {}; 
export default CreateRibbon; 
if (isHeatmap) { 
vertexShader += "varying vec3 v_realtexcoord;\n"; 
fragmentShader += "varying vec3 v_realtexcoord;\n"; 
fragmentShader += "uniform vec3 gltf_heatmapPosition;\n"; 
fragmentShader += "uniform vec3 gltf_heatmapAxel;\n"; 
fragmentShader += "uniform vec4 gltf_heatmapIntensity;\n"; 
fragmentShader += "uniform vec2 gltf_heatmapGlobalIntensity;\n"; 
fragmentShader += "uniform sampler2D gltf_heatmapTexture;\n"; 
vertexShaderMain += "v_realtexcoord = a_position;\n"; 
} 
if (isHeatmap) { 
if (defined(generatedMaterialValues.u_baseColorTexture)) { 
51fragmentShaderMain += 
"float intensity = computeIntensity(v_realtexcoord,gltf_heatmapPosition,gltf_heatmapAxel);\n"; 
fragmentShaderMain += 
"float factor = (intensity-gltf_heatmapGlobalIntensity.g)/(gltf_heatmapGlobalIntensity.r
gltf_heatmapGlobalIntensity.g);\n"; 
} 
} 
if (isHeatmap && defined(generatedMaterialValues.u_baseColorTexture)) { 
fragmentShader += 
"float computeIntensity(vec3 position,vec3 originPosition,vec3 region) \n" + 
"{\n" + 
" float x = (position.x-originPosition.x + region.x)/(region.x*2.0);\n" + 
" float y = (position.y-originPosition.y + region.y)/(region.y*2.0);\n" + 
" float z = (position.z-originPosition.z + region.z)/(region.z*2.0);\n" + 
" float repeat_x = gltf_heatmapIntensity.r;\n" + 
" float repeat_y = gltf_heatmapIntensity.g;\n" + 
" float intensityRange = gltf_heatmapIntensity.b - gltf_heatmapIntensity.a;\n" + 
" float down = floor(z*repeat_x*repeat_y);\n" + 
" float col = mod(down,floor(repeat_x));\n" + 
" float realCol = floor(col/4.0);\n" + 
" float indexPipe = mod(col,4.0); \n" + 
" float row = floor(down/repeat_x);\n" + 
" vec4 intensityColor = texture2D(u_baseColorTexture, vec2(4.0*(x/repeat_x+realCol/repeat_x),1.0- 
(y/repeat_y + row/repeat_y)));\n" + 
" float intensityd;\n" + 
" if(indexPipe < 1.0){ \n" + 
" intensityd = intensityColor.r; \n" + 
" }else if(indexPipe <2.0){ \n" + 
" intensityd = intensityColor.g; \n" + 
" }else if(indexPipe <3.0){ \n" + 
" intensityd = intensityColor.b; \n" + 
" }else{ \n" + 
" intensityd = intensityColor.a; \n" + 
" } \n" + 
" float col1 = mod((down+1.0),repeat_x);\n" + 
" float realCol1 = floor(col1/4.0);\n" + 
" float indexPipe1 = mod(col1,4.0); \n" + 
" float row1 = floor((down+1.0)/repeat_x);\n" + 
" vec4 intensityuColor = texture2D(u_baseColorTexture, vec2(4.0*(x/repeat_x+realCol1/repeat_x),1.0- 
(y/repeat_y + row1/repeat_y)));\n" + 
" float intensityu;\n" + 
" if(indexPipe1 < 1.0){ \n" + 
" intensityu = intensityuColor.r; \n" + 
" }else if(indexPipe1 <2.0){ \n" + 
" intensityu = intensityuColor.g; \n" + 
" }else if(indexPipe1 <3.0){ \n" + 
" intensityu = intensityuColor.b; \n" + 
" }else{ \n" + 
" intensityu = intensityuColor.a; \n" + 
" } \n" + 
" float real_intensity = intensityd + (z*repeat_x*repeat_y - down)*(intensityu - intensityd);\n" + 
" real_intensity = real_intensity*intensityRange + gltf_heatmapIntensity.a;\n" + 
" return real_intensity;\n" + 
// ' return 1.0;\n' + 
"}\n"; 
} 
53
