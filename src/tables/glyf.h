#ifndef CARYLL_TABLES_GLYF_H
#define CARYLL_TABLES_GLYF_H

#include <support/util.h>
#include <font/caryll-sfnt.h>
#include <support/glyphorder.h>

#include "head.h"
#include "maxp.h"

enum GlyphType { SIMPLE, COMPOSITE };

typedef struct {
	float x;
	float y;
	int8_t onCurve;
} glyf_point;

typedef struct {
	uint16_t pointsCount;
	glyf_point *points;
} glyf_contour;

// CFF stems and hint masks
typedef struct {
	bool isEdge;
	float position;
	float width;
} hint_stemH, hint_stemV;

typedef struct {
	uint16_t contour;
	uint16_t point;
	bool maskH[0x100];
	bool maskV[0x100];
} hint_mask, hint_contour_mask;

typedef struct {
	glyph_handle glyph;
	// transformation term
	float a;
	float b;
	float c;
	float d;
	float x;
	float y;
	// flags
	bool roundToGrid;
	bool useMyMetrics;
} glyf_reference;

typedef struct {
	float xMin;
	float xMax;
	float yMin;
	float yMax;
	uint16_t nestDepth;
	uint16_t nPoints;
	uint16_t nContours;
	uint16_t nCompositePoints;
	uint16_t nCompositeContours;
} glyf_glyph_stat;

typedef struct {
	sds name;

	// Metrics
	uint16_t advanceWidth;
	uint16_t advanceHeight;
	float verticalOrigin;

	// Outline
	// NOTE: SFNT does not support mixed glyphs, but we do.
	uint16_t numberOfContours;
	uint16_t numberOfReferences;
	glyf_contour *contours;
	glyf_reference *references;
	
	// Postscript hints
	uint16_t numberOfStemH;
	uint16_t numberOfStemV;
	uint16_t numberOfHintMasks;
	uint16_t numberOfContourMasks;
	hint_stemH *stemH;
	hint_stemV *stemV;
	hint_mask *hintMasks;
	hint_contour_mask *contourMasks;
	
	// TTF instructions
	uint16_t instructionsLength;
	uint8_t *instructions;
	
	// Stats
	glyf_glyph_stat stat;
} glyf_glyph;

typedef struct {
	uint16_t numberGlyphs;
	glyf_glyph **glyphs;
} table_glyf;

static const int GLYF_FLAG_ON_CURVE = 1;
static const int GLYF_FLAG_X_SHORT = (1 << 1);
static const int GLYF_FLAG_Y_SHORT = (1 << 2);
static const int GLYF_FLAG_REPEAT = (1 << 3);
static const int GLYF_FLAG_SAME_X = (1 << 4);
static const int GLYF_FLAG_SAME_Y = (1 << 5);
static const int GLYF_FLAG_POSITIVE_X = (1 << 4);
static const int GLYF_FLAG_POSITIVE_Y = (1 << 5);

static const int ARG_1_AND_2_ARE_WORDS = (1 << 0);
static const int ARGS_ARE_XY_VALUES = (1 << 1);
static const int ROUND_XY_TO_GRID = (1 << 2);
static const int WE_HAVE_A_SCALE = (1 << 3);
static const int MORE_COMPONENTS = (1 << 5);
static const int WE_HAVE_AN_X_AND_Y_SCALE = (1 << 6);
static const int WE_HAVE_A_TWO_BY_TWO = (1 << 7);
static const int WE_HAVE_INSTRUCTIONS = (1 << 8);
static const int USE_MY_METRICS = (1 << 9);
static const int OVERLAP_COMPOUND = (1 << 10);

glyf_glyph *caryll_new_glyf_glhph();
table_glyf *caryll_read_glyf(caryll_packet packet, table_head *head, table_maxp *maxp);
void caryll_delete_glyf(table_glyf *table);
void caryll_glyf_to_json(table_glyf *table, json_value *root, caryll_dump_options *dumpopts);
table_glyf *caryll_glyf_from_json(json_value *root, glyph_order_hash glyph_order,
                                  caryll_dump_options *dumpopts);
void caryll_write_glyf(table_glyf *table, table_head *head, caryll_buffer *bufglyf,
                       caryll_buffer *bufloca);

#endif
