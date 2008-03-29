#ifndef __TREMODEL_H__
#define __TREMODEL_H__

#include <TCFoundation/TCAlertSender.h>
#include <TCFoundation/TCTypedObjectArray.h>
#include <TCFoundation/TCTypedValueArray.h>
#include <TRE/TREShapeGroup.h>
#include <TRE/TREColoredShapeGroup.h>
#include <TRE/TREVertexKey.h>
#include <TRE/TRESmoother.h>
#include <TCFoundation/TCVector.h>
#include <TCFoundation/TCStlIncludes.h>

struct TREVertex;
class TRESubModel;
class TREMainModel;
class TREColoredShapeGroup;
class TREVertexArray;

class TRENormalInfo : public TCAlertSender
{
public:
	TRENormalInfo(void)
		:m_normals(NULL),
		m_normalIndex(-1),
		m_smoother(NULL),
		m_smootherIndex(-1) {};

	TREVertexArray *m_normals;
	int m_normalIndex;
	TRESmoother *m_smoother;
	int m_smootherIndex;
};

typedef TCTypedObjectArray<TRESubModel> TRESubModelArray;
typedef TCTypedObjectArray<TREShapeGroup> TREShapeGroupArray;
typedef TCTypedObjectArray<TREColoredShapeGroup> TREColoredShapeGroupArray;
typedef TCTypedObjectArray<TRENormalInfo> TRENormalInfoArray;
typedef std::map<TREVertexKey, TRESmoother> TREConditionalMap;

typedef enum
{
	TREMStandard,
	TREMFirst = TREMStandard,
	TREMLines,
	TREMEdgeLines,
	TREMConditionalLines,
	TREMStud,
	TREMBFC,
	TREMStudBFC,
	TREMTransparent,
	TREMLast = TREMTransparent
} TREMSection;

class TREModel : public TCAlertSender
{
public:
	TREModel(void);
	TREModel(const TREModel &other);
	TREModel(const TREModel &other, bool shallow);
	virtual TCObject *copy(void);
	virtual TREModel *shallowCopy(void);
	virtual void setMainModel(TREMainModel *mainModel)
	{
		m_mainModel = mainModel;
	}
	virtual TREMainModel *getMainModel(void) const { return m_mainModel; }
	virtual void setName(const char *name);
	virtual const char *getName(void) const { return m_name; }
//	virtual TREVertexArray *getVertices(void) { return m_vertices; }
	virtual TRESubModel *addSubModel(TCFloat *matrix, TREModel *model,
		bool invert);
	virtual TRESubModel *addSubModel(TCULong color, TCULong edgeColor,
		TCFloat *matrix, TREModel *model, bool invert);
	virtual void addLine(const TCVector *vertices);
	virtual void addEdgeLine(const TCVector *vertices);
	virtual void addConditionalLine(const TCVector *vertices,
		const TCVector *controlPoints);
	virtual void addConditionalLine(const TCVector &p1, const TCVector &p2,
		const TCVector &c1, const TCVector &c2);
	virtual void addLine(TCULong color, const TCVector *vertices);
	virtual void addTriangle(const TCVector *vertices);
	virtual void addTriangle(const TCVector *vertices,
		const TCVector *normals);
	virtual void addTriangle(TCULong color, const TCVector *vertices);
	virtual void addBFCTriangle(const TCVector *vertices);
	virtual void addBFCTriangle(const TCVector *vertices,
		const TCVector *normals);
	virtual void addBFCTriangle(TCULong color, const TCVector *vertices);
	virtual void addQuad(const TCVector *vertices);
	virtual void addQuad(TCULong color, const TCVector *vertices);
	virtual void addBFCQuad(const TCVector *vertices);
	virtual void addBFCQuad(TCULong color, const TCVector *vertices);
	virtual void addQuadStrip(const TCVector *vertices, const TCVector *normals,
		int count, bool flat = false);
	virtual void addTriangleStrip(const TCVector *vertices,
		const TCVector *normals, int count, bool flat = false);
	virtual void addQuadStrip(TREShapeGroup *shapeGroup,
		const TCVector *vertices, const TCVector *normals, int count,
		bool flat);
	virtual void addBFCQuadStrip(const TCVector *vertices,
		const TCVector *normals, int count, bool flat = false);
	virtual void addQuadStrip(TCULong color, const TCVector *vertices,
		const TCVector *normals, int count, bool flat = false);
	virtual void addQuadStrip(TREColoredShapeGroup *shapeGroup, TCULong color,
		const TCVector *vertices, const TCVector *normals, int count,
		bool flat = false);
//	virtual void addTriangleStrip(TREColoredShapeGroup *shapeGroup,
//		TCULong color, TCVector *vertices, TCVector *normals, int count,
//		bool flat = false);
	virtual void addTriangleStrip(TREShapeGroup *shapeGroup,
		const TCVector *vertices, const TCVector *normals, int count,
		bool flat = false);
	virtual void addBFCQuadStrip(TCULong color, const TCVector *vertices,
		const TCVector *normals, int count, bool flat = false);
//	virtual void addBFCTriangleStrip(TCULong color, TCVector *vertices,
//		TCVector *normals, int count, bool flat = false);
	virtual void addBFCTriangleStrip(const TCVector *vertices,
		const TCVector *normals, int count, bool flat = false);
	virtual void addTriangleFan(const TCVector *vertices,
		const TCVector *normals, const TCVector *textureCoords, int count,
		bool flat = false);
	virtual void addTriangleFan(TREShapeGroup *shapeGroup,
		const TCVector *vertices, const TCVector *normals,
		const TCVector *textureCoords, int count, bool flat = false);
	virtual void addBFCTriangleFan(const TCVector *vertices,
		const TCVector *normals, const TCVector *textureCoords, int count,
		bool flat = false);
	virtual void addTriangleFan(TCULong color, const TCVector *vertices,
		const TCVector *normals, int count, bool flat = false);
	virtual void addTriangleFan(TREColoredShapeGroup *shapeGroup, TCULong color,
		const TCVector *vertices, const TCVector *normals, int count,
		bool flat = false);
	virtual void addBFCTriangleFan(TCULong color, const TCVector *vertices,
		const TCVector *normals, int count, bool flat = false);
	virtual void compile(TREMSection section, bool colored,
		bool nonUniform = false);
	virtual void draw(TREMSection section);
	virtual void draw(TREMSection section, bool colored,
		bool subModelsOnly = false, bool nonUniform = false);
	virtual void drawColored(TREMSection section);
	virtual void setPartFlag(bool value) { m_flags.part = value; }
	virtual bool isPart(void) { return m_flags.part != false; }
	virtual void setNoShrinkFlag(bool value) { m_flags.noShrink = value; }
	virtual bool getNoShrinkFlag(void) { return m_flags.noShrink != false; }
	virtual bool isFlattened(void) { return m_flags.flattened != false; }
	virtual void flatten(void);
	virtual void smooth(void);
	virtual void addCylinder(const TCVector &center, TCFloat radius, TCFloat height,
		int numSegments, int usedSegments = -1, bool bfc = false);
	virtual void addSlopedCylinder(const TCVector &center, TCFloat radius,
		TCFloat height, int numSegments, int usedSegments = -1, bool bfc = false);
	virtual void addSlopedCylinder2(const TCVector &center, TCFloat radius,
		TCFloat height, int numSegments, int usedSegments = -1, bool bfc = false);
	virtual void addStudDisc(const TCVector &center, TCFloat radius,
		int numSegments, int usedSegments = -1, bool bfc = false);
	virtual void addChrd(const TCVector &center, TCFloat radius, int numSegments,
		int usedSegments = -1, bool bfc = false);
	virtual void addDisc(const TCVector &center, TCFloat radius, int numSegments,
		int usedSegments = -1, bool bfc = false, bool stud = false);
	virtual void addNotDisc(const TCVector &center, TCFloat radius,
		int numSegments, int usedSegments = -1, bool bfc = false);
	virtual void addCone(const TCVector &center, TCFloat radius, TCFloat height,
		int numSegments, int usedSegments = -1, bool bfc = false);
	virtual void addEighthSphere(const TCVector& center, TCFloat radius,
		int numSegments, bool bfc);
	virtual void addTorusIO(bool inner, const TCVector& center, TCFloat yRadius,
		TCFloat xzRadius, int numSegments, int usedSegments, bool bfc);
	virtual void addOpenCone(const TCVector &center, TCFloat radius1,
		TCFloat radius2, TCFloat height, int numSegments, int usedSegments = -1,
		bool bfc = false);
	virtual void addCircularEdge(const TCVector &center, TCFloat radius,
		int numSegments, int usedSegments = -1);
	virtual void addRing(const TCVector &center, TCFloat radius1, TCFloat radius2,
		int numSegments, int usedSegments = -1, bool bfc = false);
	virtual void addOpenConeConditionals(TCVector *points, int numSegments,
		int usedSegments);
	virtual void addSlopedCylinder2Conditionals(TCVector *points,
		int numSegments, int usedSegments);
	virtual void addTorusIOConditionals(bool innder, TCVector *points,
		int numSegments, int usedSegments, const TCVector& center, TCFloat radius,
		TCFloat height);
	virtual void addEighthSphereConditionals(TCVector *points, int numSegments);
	TCVector calcIntersection(int i, int j, int num, TCVector* zeroXPoints,
		TCVector* zeroYPoints, TCVector* zeroZPoints);
	virtual void getBoundingBox(TCVector& min, TCVector& max);
	virtual void scanPoints(TCObject *scanner,
		TREScanPointCallback scanPointCallback, const TCFloat *matrix);
	virtual void unshrinkNormals(const TCFloat *scaleMatrix);
	void unshrinkNormals(const TCFloat *matrix, const TCFloat *unshrinkMatrix);
	TREModel *getUnMirroredModel(void);
	TREModel *getInvertedModel(void);
	virtual void uncompile(void);
	virtual void step(void) { m_curStepIndex++; }
	virtual int getCurStepIndex(void) const { return m_curStepIndex; }
	bool isLineSection(int section)
	{
		return isLineSection((TREMSection)section);
	}
	bool isLineSection(TREMSection section)
	{
		return section == TREMLines || section == TREMEdgeLines;
	}
	virtual void transferColoredTransparent(TREMSection section,
		const TCFloat *matrix);
	virtual void transferTransparent(TCULong color, TREMSection section,
		const TCFloat *matrix);
	virtual void cleanupTransparent(TREMSection section);
	virtual TCObject *getAlertSender(void);
	virtual int saveSTL(void);
protected:
	virtual ~TREModel(void);
	virtual void dealloc(void);
	virtual void setup(TREMSection section);
	virtual void setupLines(void);
	virtual void setupStandard(void);
	virtual void setupStud(void);
	virtual void setupStudBFC(void);
	virtual void setupBFC(void);
	virtual void setupColored(TREMSection section);
	virtual void setupColored(void);
	virtual void setupColoredStud(void);
	virtual void setupColoredStudBFC(void);
	virtual void setupColoredLines(void);
	virtual void setupColoredBFC(void);
	virtual void setupEdges(void);
	virtual void setupColoredEdges(void);
	virtual void setupConditional(void);
	virtual void setupColoredConditional(void);
	virtual void flatten(TREModel *model, const TCFloat *matrix, TCULong color,
		bool colorSet, TCULong edgeColor, bool edgeColorSet,
		bool includeShapes);
	virtual void checkGLError(char *msg);
/*
	virtual void flattenShapes(TREShapeGroup *dstShapes,
		TREShapeGroup *srcShapes, TCFloat *matrix, TCULong color, bool colorSet);
	virtual void flattenShapes(TREVertexArray *dstVertices,
		TREVertexArray *dstNormals,
		TCULongArray *dstColors,
		TCULongArray *dstIndices,
		TCULongArray *dstCPIndices,
		TREVertexArray *srcVertices,
		TREVertexArray *srcNormals,
		TCULongArray *srcColors,
		TCULongArray *srcIndices,
		TCULongArray *srcCPIndices,
		TCFloat *matrix,
		TCULong color,
		bool colorSet);
	virtual void flattenStrips(TREVertexArray *dstVertices,
		TREVertexArray *dstNormals, TCULongArray *dstColors,
		TCULongArray *dstIndices, TCULongArray *dstStripCounts,
		TREVertexArray *srcVertices, TREVertexArray *srcNormals,
		TCULongArray *srcColors, TCULongArray *srcIndices,
		TCULongArray *srcStripCounts, TCFloat *matrix, TCULong color,
		bool colorSet);
*/
	void setCirclePoint(TCFloat angle, TCFloat radius, const TCVector& center,
		TCVector& point);
	void scanBoundingBoxPoint(const TCVector &point);
	virtual void calculateBoundingBox(void);
	virtual void quadStripToQuad(int index, const TCVector *stripVertices,
		const TCVector *stripNormals, TCVector *quadVertices,
		TCVector *quadNormals);
	virtual void triangleStripToTriangle(int index,
		const TCVector *stripVertices, const TCVector *stripNormals,
		TCVector *triangleVertices, TCVector *triangleNormals);
	virtual void triangleFanToTriangle(int index, const TCVector *fanVertices,
		const TCVector *fanNormals, const TCVector *fanTextureCoords,
		TCVector *triangleVertices, TCVector *triangleNormals,
		TCVector *triangleTextureCoords);
	virtual void unMirror(TREModel *originalModel);
	virtual void invert(TREModel *originalModel);
	virtual bool checkShapeGroupPresent(TREShapeGroup *shapeGroup,
		TREMSection section, bool colored);
	virtual bool checkSectionPresent(TREMSection section);
	virtual bool checkColoredSectionPresent(TREMSection section);
	virtual bool checkSectionPresent(TREMSection section, bool colored);
	virtual int sphereIndex(int i, int j, int usedSegments);
/*
	virtual bool checkDefaultColorPresent(void);
	virtual bool checkStudsPresent(void);
	virtual bool checkBFCPresent(void);
	virtual bool checkDefaultColorLinesPresent(void);
	virtual bool checkEdgeLinesPresent(void);
	virtual bool checkConditionalLinesPresent(void);
	virtual bool checkColoredPresent(void);
	virtual bool checkColoredBFCPresent(void);
	virtual bool checkColoredLinesPresent(void);
	virtual bool checkColoredEdgeLinesPresent(void);
	virtual bool checkColoredConditionalLinesPresent(void);
*/
	virtual void setSectionPresent(TREMSection section, bool colored);
	virtual bool isSectionPresent(TREMSection section, bool colored);
	virtual bool shouldLoadConditionalLines(void);
	virtual void genStudTextureCoords(TCVector *textureCoords, int vertexCount);
	virtual void fillConditionalMap(TREConditionalMap &conditionalMap);
	virtual void fillConditionalMap(TREConditionalMap &conditionalMap,
		TREShapeGroup *shapeGroup);
	virtual void addConditionalPoint(TREConditionalMap &conditionalMap,
		const TREVertexArray *vertices, int index0, int index1,
		const TREVertexKey &vertexKey);
	virtual void calcShapeNormals(TREConditionalMap &conditionalMap,
		TRENormalInfoArray *normalInfos, TREShapeType shapeType);
	virtual void calcShapeNormals(TREConditionalMap &conditionalMap,
		TRENormalInfoArray *normalInfos, TREMSection section,
		TREShapeType shapeType);
	virtual void calcShapeNormals(TREConditionalMap &conditionalMap,
		TRENormalInfoArray *normalInfos, TREShapeGroup *shapeGroup,
		TREShapeType shapeType);
	virtual void processSmoothEdge(TREConditionalMap &conditionalMap,
		TRENormalInfoArray *normalInfos, const TREVertexArray *vertices,
		TREVertexArray *normals, int index0, int index1, int index2);
	virtual int getConditionalLine(TREConditionalMap &conditionalMap,
		const TREVertex point0, const TREVertex point1, TRESmoother *&smoother);
	void applyShapeNormals(TRENormalInfoArray *normalInfos);
	void finishShapeNormals(TREConditionalMap &conditionalMap);
	void flattenNonUniform(void);
	void flattenConditionals(const TCFloat *matrix, TCULong edgeColor,
		bool edgeColorSet);
	void removeConditionals(void);
	void findLights(void);
	void findLights(float *matrix);
	void calcTangentControlPoint(TCVector &controlPoint, int index,
		int numSegments);
//	void flattenNonUniform(TCULong color, bool colorSet, TCULong edgeColor,
//		bool edgeColorSet);

	static void setGlNormalize(bool value);
	static int printStlTriangle(TREVertexArray *vertices,
		TCULongArray *indices, int ix, int i0, int i1, int i2);

	char *m_name;
	TREMainModel *m_mainModel;
	TRESubModelArray *m_subModels;
	TREShapeGroup *m_shapes[TREMLast + 1];
	TREColoredShapeGroup *m_coloredShapes[TREMLast + 1];
	GLuint m_listIDs[TREMLast + 1];
	GLuint m_coloredListIDs[TREMLast + 1];
	TREModel *m_unMirroredModel;
	TREModel *m_invertedModel;
	TCULong m_sectionsPresent;
	TCULong m_coloredSectionsPresent;
	TCVector m_boundingMin;
	TCVector m_boundingMax;
	int m_curStepIndex;
	struct
	{
		bool part:1;
		bool noShrink:1;
		bool boundingBox:1;
		bool unshrunkNormals:1;
		bool unMirrored:1;
		bool inverted:1;
		bool flattened:1;
	} m_flags;
};

#endif // __TREMODEL_H__
