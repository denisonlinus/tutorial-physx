#include "CCapsuleSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "S3DVertex.h"

namespace irr
{
namespace scene
{

	f32 SphereVertices[] = 
	{
		0.0026, -0.499124, 0.00346, 
		0.217415, -0.43266, -0.120563, 
		0.250647, -0.43266, 0.00346, 
		0.0026, 0.493064, 0.00346, 
		0.250647, 0.4266, 0.00346, 
		0.217415, 0.4266, -0.120563, 
		0.0026, -0.499124, 0.00346, 
		0.126624, -0.43266, -0.211355, 
		0.0026, 0.493064, 0.00346, 
		0.126624, 0.4266, -0.211355, 
		0.0026, -0.499124, 0.00346, 
		0.0026, -0.43266, -0.244587, 
		0.0026, 0.493064, 0.00346, 
		0.0026, 0.4266, -0.244587, 
		0.0026, -0.499124, 0.00346, 
		-0.121423, -0.43266, -0.211355, 
		0.0026, 0.493064, 0.00346, 
		-0.121423, 0.4266, -0.211355, 
		0.0026, -0.499124, 0.00346, 
		-0.212215, -0.43266, -0.120563, 
		0.0026, 0.493064, 0.00346, 
		-0.212215, 0.4266, -0.120563, 
		0.0026, -0.499124, 0.00346, 
		-0.245447, -0.43266, 0.00346, 
		0.0026, 0.493064, 0.00346, 
		-0.245447, 0.4266, 0.00346, 
		0.0026, -0.499124, 0.00346, 
		-0.212215, -0.43266, 0.127484, 
		0.0026, 0.493064, 0.00346, 
		-0.212215, 0.4266, 0.127484, 
		0.0026, -0.499124, 0.00346, 
		-0.121423, -0.43266, 0.218275, 
		0.0026, 0.493064, 0.00346, 
		-0.121423, 0.4266, 0.218275, 
		0.0026, -0.499124, 0.00346, 
		0.0026, -0.43266, 0.251507, 
		0.0026, 0.493064, 0.00346, 
		0.0026, 0.4266, 0.251507, 
		0.0026, -0.499124, 0.00346, 
		0.126624, -0.43266, 0.218275, 
		0.0026, 0.493064, 0.00346, 
		0.126624, 0.4266, 0.218275, 
		0.0026, -0.499124, 0.00346, 
		0.217415, -0.43266, 0.127484, 
		0.0026, 0.493064, 0.00346, 
		0.217415, 0.4266, 0.127484, 
		0.0026, -0.499124, 0.00346, 
		0.250647, -0.43266, 0.00346, 
		0.0026, 0.493064, 0.00346, 
		0.250647, 0.4266, 0.00346, 
		0.374671, -0.251077, -0.211355, 
		0.43223, -0.251077, 0.00346, 
		0.217415, -0.251077, -0.36861, 
		0.0026, -0.251077, -0.42617, 
		-0.212215, -0.251077, -0.36861, 
		-0.36947, -0.251077, -0.211355, 
		-0.427029, -0.251077, 0.00346, 
		-0.36947, -0.251077, 0.218275, 
		-0.212215, -0.251077, 0.37553, 
		0.0026, -0.251077, 0.43309, 
		0.217415, -0.251077, 0.37553, 
		0.374671, -0.251077, 0.218275, 
		0.43223, -0.251077, 0.00346, 
		0.498694, -0.00303, 0.00346, 
		0.43223, -0.00303, -0.244587, 
		0.250647, -0.00303, -0.42617, 
		0.0026, -0.00303, -0.492634, 
		-0.245447, -0.00303, -0.42617, 
		-0.427029, -0.00303, -0.244587, 
		-0.493493, -0.00303, 0.00346, 
		-0.427029, -0.00303, 0.251507, 
		-0.245447, -0.00303, 0.43309, 
		0.0026, -0.00303, 0.499554, 
		0.250647, -0.00303, 0.43309, 
		0.43223, -0.00303, 0.251507, 
		0.498694, -0.00303, 0.00346, 
		0.374671, 0.245017, -0.211355, 
		0.43223, 0.245017, 0.00346, 
		0.217415, 0.245017, -0.36861, 
		0.0026, 0.245017, -0.42617, 
		-0.212215, 0.245017, -0.36861, 
		-0.36947, 0.245017, -0.211355, 
		-0.427029, 0.245017, 0.00346, 
		-0.36947, 0.245017, 0.218275, 
		-0.212215, 0.245017, 0.37553, 
		0.0026, 0.245017, 0.43309, 
		0.217415, 0.245017, 0.37553, 
		0.374671, 0.245017, 0.218275, 
		0.43223, 0.245017, 0.00346
	};

	f32 SphereNormals[] =
	{
		0, -1, -0, 
		0.431481, -0.867044, -0.249116, 
		0.573584, -0.819147, 0, 
		0, 1, 0, 
		0.573584, 0.819147, 0, 
		0.431481, 0.867044, -0.249116, 
		0, -1, -0, 
		0.286792, -0.819147, -0.496738, 
		0, 1, 0, 
		0.286792, 0.819147, -0.496738, 
		0, -1, -0, 
		0, -0.867044, -0.498231, 
		0, 1, 0, 
		0, 0.867044, -0.498231, 
		0, -1, -0, 
		-0.286792, -0.819147, -0.496738, 
		0, 1, 0, 
		-0.286792, 0.819147, -0.496738, 
		0, -1, -0, 
		-0.431481, -0.867044, -0.249116, 
		0, 1, 0, 
		-0.431481, 0.867044, -0.249116, 
		0, -1, -0, 
		-0.573584, -0.819147, 0, 
		0, 1, 0, 
		-0.573584, 0.819147, 0, 
		0, -1, -0, 
		-0.431481, -0.867044, 0.249116, 
		0, 1, 0, 
		-0.431481, 0.867044, 0.249116, 
		0, -1, -0, 
		-0.286792, -0.819147, 0.496738, 
		0, 1, 0, 
		-0.286792, 0.819147, 0.496738, 
		0, -1, -0, 
		0, -0.867044, 0.498231, 
		0, 1, 0, 
		0, 0.867044, 0.498231, 
		0, -1, -0, 
		0.286792, -0.819147, 0.496738, 
		0, 1, 0, 
		0.286792, 0.819147, 0.496738, 
		0, -1, -0, 
		0.431481, -0.867044, 0.249116, 
		0, 1, 0, 
		0.431481, 0.867044, 0.249116, 
		0, -1, -0, 
		0.573584, -0.819147, 0, 
		0, 1, 0, 
		0.573584, 0.819147, 0, 
		0.749141, -0.501714, -0.432517, 
		0.865033, -0.501715, 0, 
		0.432517, -0.501714, -0.749141, 
		-0, -0.501714, -0.865033, 
		-0.432517, -0.501714, -0.749141, 
		-0.749141, -0.501714, -0.432517, 
		-0.865033, -0.501714, 0, 
		-0.749141, -0.501714, 0.432517, 
		-0.432517, -0.501714, 0.749141, 
		0, -0.501714, 0.865033, 
		0.432517, -0.501714, 0.749141, 
		0.749141, -0.501714, 0.432517, 
		0.865033, -0.501715, 0, 
		1, 0, 0, 
		0.866025, 0, -0.5, 
		0.5, 0, -0.866025, 
		0, 0, -1, 
		-0.5, 0, -0.866025, 
		-0.866025, 0, -0.5, 
		-1, 0, 0, 
		-0.866025, 0, 0.5, 
		-0.5, 0, 0.866025, 
		0, 0, 1, 
		0.5, 0, 0.866025, 
		0.866025, 0, 0.5, 
		1, 0, 0, 
		0.749141, 0.501714, -0.432517, 
		0.865033, 0.501714, 0, 
		0.432517, 0.501714, -0.749141, 
		-0, 0.501714, -0.865033, 
		-0.432517, 0.501714, -0.749141, 
		-0.749141, 0.501714, -0.432517, 
		-0.865033, 0.501714, 0, 
		-0.749141, 0.501714, 0.432517, 
		-0.432517, 0.501714, 0.749141, 
		-0, 0.501714, 0.865033, 
		0.432517, 0.501714, 0.749141, 
		0.749141, 0.501714, 0.432517, 
		0.865033, 0.501714, 0
	};
	f32 SphereTextureMap[] =
	{
		0, 1,
		0.083333, 0.833333, 
		0, 0.833333, 
		0, 0, 
		0, 0.166667, 
		0.083333, 0.166667, 
		0.083333, 1, 
		0.166667, 0.833333, 
		0.083333, 0, 
		0.166667, 0.166667, 
		0.166667, 1, 
		0.25, 0.833333, 
		0.166667, 0, 
		0.25, 0.166667, 
		0.25, 1, 
		0.333333, 0.833333, 
		0.25, 0, 
		0.333333, 0.166667, 
		0.333333, 1, 
		0.416667, 0.833333, 
		0.333333, 0, 
		0.416667, 0.166667, 
		0.416667, 1, 
		0.5, 0.833333, 
		0.416667, 0, 
		0.5, 0.166667, 
		0.5, 1, 
		0.583333, 0.833333, 
		0.5, 0, 
		0.583333, 0.166667, 
		0.583333, 1, 
		0.666667, 0.833333, 
		0.583333, 0, 
		0.666667, 0.166667, 
		0.666667, 1, 
		0.75, 0.833333, 
		0.666667, 0, 
		0.75, 0.166667, 
		0.75, 1, 
		0.833333, 0.833333, 
		0.75, 0, 
		0.833333, 0.166667, 
		0.833333, 1, 
		0.916667, 0.833333, 
		0.833333, 0, 
		0.916667, 0.166667, 
		0.916667, 1, 
		1, 0.833333, 
		0.916667, 0, 
		1, 0.166667, 
		0.083333, 0.666667, 
		0, 0.666667, 
		0.166667, 0.666667, 
		0.25, 0.666667, 
		0.333333, 0.666667, 
		0.416667, 0.666667, 
		0.5, 0.666667, 
		0.583333, 0.666667, 
		0.666667, 0.666667, 
		0.75, 0.666667, 
		0.833333, 0.666667, 
		0.916667, 0.666667, 
		1, 0.666667, 
		0, 0.5, 
		0.083333, 0.5, 
		0.166667, 0.5, 
		0.25, 0.5, 
		0.333333, 0.5, 
		0.416667, 0.5, 
		0.5, 0.5, 
		0.583333, 0.5, 
		0.666667, 0.5, 
		0.75, 0.5, 
		0.833333, 0.5, 
		0.916667, 0.5, 
		1, 0.5, 
		0.083333, 0.333333, 
		0, 0.333333, 
		0.166667, 0.333333, 
		0.25, 0.333333, 
		0.333333, 0.333333, 
		0.416667, 0.333333, 
		0.5, 0.333333, 
		0.583333, 0.333333, 
		0.666667, 0.333333, 
		0.75, 0.333333, 
		0.833333, 0.333333, 
		0.916667, 0.333333, 
		1, 0.333333
	};

	s32 SphereIndices[] = 
	{
		0, 1, 2, 3, 4, 5, 6, 7, 1, 8, 5, 9, 10, 11, 7, 12, 9, 13, 14, 15, 11, 16, 13, 17, 18, 19, 15, 20, 17, 21, 22, 23, 19, 24, 21, 25, 26, 27, 23, 28, 25, 29, 30, 31, 27, 32, 29, 33, 34, 35, 31, 36, 33, 37, 38, 39, 35, 40, 37, 41, 42, 43, 39, 44, 41, 45, 46, 47, 43, 48, 45, 49, 2, 50, 51, 2, 1, 50, 1, 7, 50, 50, 7, 52, 7, 53, 52, 7, 11, 53, 11, 15, 53, 53, 15, 54, 15, 55, 54, 15, 19, 55, 19, 23, 55, 55, 23, 56, 23, 57, 56, 23, 27, 57, 27, 31, 57, 57, 31, 58, 31, 59, 58, 31, 35, 59, 35, 39, 59, 59, 39, 60, 39, 61, 60, 39, 43, 61, 43, 47, 61, 61, 47, 62, 51, 50, 63, 63, 50, 64, 50, 65, 64, 50, 52, 65, 52, 53, 65, 65, 53, 66, 53, 67, 66, 53, 54, 67, 54, 55, 67, 67, 55, 68, 55, 69, 68, 55, 56, 69, 56, 57, 69, 69, 57, 70, 57, 71, 70, 57, 58, 71, 58, 59, 71, 71, 59, 72, 59, 73, 72, 59, 60, 73, 60, 61, 73, 73, 61, 74, 61, 75, 74, 61, 62, 75, 63, 76, 77, 63, 64, 76, 64, 65, 76, 76, 65, 78, 65, 79, 78, 65, 66, 79, 66, 67, 79, 79, 67, 80, 67, 81, 80, 67, 68, 81, 68, 69, 81, 81, 69, 82, 69, 83, 82, 69, 70, 83, 70, 71, 83, 83, 71, 84, 71, 85, 84, 71, 72, 85, 72, 73, 85, 85, 73, 86, 73, 87, 86, 73, 74, 87, 74, 75, 87, 87, 75, 88, 77, 76, 4, 4, 76, 5, 76, 9, 5, 76, 78, 9, 78, 79, 9, 9, 79, 13, 79, 17, 13, 79, 80, 17, 80, 81, 17, 17, 81, 21, 81, 25, 21, 81, 82, 25, 82, 83, 25, 25, 83, 29, 83, 33, 29, 83, 84, 33, 84, 85, 33, 33, 85, 37, 85, 41, 37, 85, 86, 41, 86, 87, 41, 41, 87, 45, 87, 49, 45, 87, 88, 49
	};

	f32 CylinderVertices[] = 
	{
		0.496892, -0.499864, -0, 
		-0.003108, -0.499864, -0, 
		0.429904, -0.499864, -0.25, 
		0.496892, 0.499864, -0, 
		0.429904, 0.499864, -0.25, 
		-0.003108, 0.499864, -0, 
		0.496892, -0.499864, -0, 
		0.429904, -0.333243, -0.25, 
		0.496892, -0.333243, -0, 
		0.429904, -0.499864, -0.25, 
		0.496892, -0.166621, -0, 
		0.429904, -0.166621, -0.25, 
		0.429904, 0, -0.25, 
		0.496892, 0, -0, 
		0.496892, 0.166621, -0, 
		0.429904, 0.166621, -0.25, 
		0.429904, 0.333243, -0.25, 
		0.496892, 0.333243, -0, 
		0.496892, 0.499864, -0, 
		0.429904, 0.499864, -0.25, 
		0.246892, -0.499864, -0.433013, 
		0.246892, 0.499864, -0.433013, 
		0.246892, -0.499864, -0.433013, 
		0.246892, -0.333243, -0.433013, 
		0.246892, -0.166621, -0.433013, 
		0.246892, 0, -0.433013, 
		0.246892, 0.166621, -0.433013, 
		0.246892, 0.333243, -0.433013, 
		0.246892, 0.499864, -0.433013, 
		-0.003108, -0.499864, -0.5, 
		-0.003108, 0.499864, -0.5, 
		-0.003108, -0.333243, -0.5, 
		-0.003108, -0.499864, -0.5, 
		-0.003108, -0.166621, -0.5, 
		-0.003108, 0, -0.5, 
		-0.003108, 0.166621, -0.5, 
		-0.003108, 0.333243, -0.5, 
		-0.003108, 0.499864, -0.5, 
		-0.253108, -0.499864, -0.433013, 
		-0.253108, 0.499864, -0.433013, 
		-0.253108, -0.499864, -0.433013, 
		-0.253108, -0.333243, -0.433013, 
		-0.253108, -0.166621, -0.433013, 
		-0.253108, 0, -0.433013, 
		-0.253108, 0.166621, -0.433013, 
		-0.253108, 0.333243, -0.433013, 
		-0.253108, 0.499864, -0.433013, 
		-0.436121, -0.499864, -0.25, 
		-0.436121, 0.499864, -0.25, 
		-0.436121, -0.333243, -0.25, 
		-0.436121, -0.499864, -0.25, 
		-0.436121, -0.166621, -0.25, 
		-0.436121, 0, -0.25, 
		-0.436121, 0.166621, -0.25, 
		-0.436121, 0.333243, -0.25, 
		-0.436121, 0.499864, -0.25, 
		-0.503108, -0.499864, -0, 
		-0.503108, 0.499864, -0, 
		-0.503108, -0.499864, -0, 
		-0.503108, -0.333243, -0, 
		-0.503108, -0.166621, -0, 
		-0.503108, 0, -0, 
		-0.503108, 0.166621, -0, 
		-0.503108, 0.333243, -0, 
		-0.503108, 0.499864, -0, 
		-0.436121, -0.499864, 0.25, 
		-0.436121, 0.499864, 0.25, 
		-0.436121, -0.333243, 0.25, 
		-0.436121, -0.499864, 0.25, 
		-0.436121, -0.166621, 0.25, 
		-0.436121, 0, 0.25, 
		-0.436121, 0.166621, 0.25, 
		-0.436121, 0.333243, 0.25, 
		-0.436121, 0.499864, 0.25, 
		-0.253108, -0.499864, 0.433013, 
		-0.253108, 0.499864, 0.433013, 
		-0.253108, -0.499864, 0.433013, 
		-0.253108, -0.333243, 0.433013, 
		-0.253108, -0.166621, 0.433013, 
		-0.253108, 0, 0.433013, 
		-0.253108, 0.166621, 0.433013, 
		-0.253108, 0.333243, 0.433013, 
		-0.253108, 0.499864, 0.433013, 
		-0.003108, -0.499864, 0.5, 
		-0.003108, 0.499864, 0.5, 
		-0.003108, -0.333243, 0.5, 
		-0.003108, -0.499864, 0.5, 
		-0.003108, -0.166621, 0.5, 
		-0.003108, 0, 0.5, 
		-0.003108, 0.166621, 0.5, 
		-0.003108, 0.333243, 0.5, 
		-0.003108, 0.499864, 0.5, 
		0.246892, -0.499864, 0.433013, 
		0.246892, 0.499864, 0.433013, 
		0.246892, -0.499864, 0.433013, 
		0.246892, -0.333243, 0.433013, 
		0.246892, -0.166621, 0.433013, 
		0.246892, 0, 0.433013, 
		0.246892, 0.166621, 0.433013, 
		0.246892, 0.333243, 0.433013, 
		0.246892, 0.499864, 0.433013, 
		0.429904, -0.499864, 0.25, 
		0.429904, 0.499864, 0.25, 
		0.429904, -0.333243, 0.25, 
		0.429904, -0.499864, 0.25, 
		0.429904, -0.166621, 0.25, 
		0.429904, 0, 0.25, 
		0.429904, 0.166621, 0.25, 
		0.429904, 0.333243, 0.25, 
		0.429904, 0.499864, 0.25, 
		0.496892, -0.499864, -0, 
		0.496892, -0.333243, -0, 
		0.496892, -0.166621, -0, 
		0.496892, 0, -0, 
		0.496892, 0.166621, -0, 
		0.496892, 0.333243, -0, 
0.496892, 0.499864, -0
	};

	f32 CylinderNormals[] =
	{
		0, -1, 0, 
		0, -1, 0, 
		0, -1, 0, 
		0, 1, 0, 
		0, 1, 0, 
		0, 1, 0, 
		0.965926, 0, -0.258819, 
		0.866025, 0, -0.5, 
		0.965926, 0, -0.258819, 
		0.866025, 0, -0.5, 
		0.965926, 0, -0.258819, 
		0.866025, 0, -0.5, 
		0.866025, 0, -0.5, 
		0.965926, 0, -0.258819, 
		0.965926, 0, -0.258819, 
		0.866025, 0, -0.5, 
		0.866025, 0, -0.5, 
		0.965926, 0, -0.258819, 
		0.965926, 0, -0.258819, 
		0.866025, 0, -0.5, 
		0, -1, 0, 
		0, 1, 0, 
		0.5, 0, -0.866026, 
		0.5, 0, -0.866026, 
		0.5, 0, -0.866026, 
		0.5, 0, -0.866026, 
		0.5, 0, -0.866026, 
		0.5, 0, -0.866025, 
		0.5, 0, -0.866025, 
		0, -1, 0, 
		0, 1, 0, 
		0, 0, -1, 
		0, 0, -1, 
		0, 0, -1, 
		0, 0, -1, 
		0, 0, -1, 
		0, 0, -1, 
		0, 0, -1, 
		0, -1, 0, 
		0, 1, 0, 
		-0.5, 0, -0.866025, 
		-0.5, 0, -0.866025, 
		-0.5, 0, -0.866025, 
		-0.5, 0, -0.866025, 
		-0.5, 0, -0.866025, 
		-0.5, 0, -0.866025, 
		-0.5, 0, -0.866025, 
		0, -1, 0, 
		0, 1, 0, 
		-0.866025, 0, -0.5, 
		-0.866025, 0, -0.5, 
		-0.866025, 0, -0.5, 
		-0.866025, 0, -0.5, 
		-0.866025, 0, -0.5, 
		-0.866025, 0, -0.5, 
		-0.866025, 0, -0.5, 
		0, -1, 0, 
		0, 1, 0, 
		-1, 0, 0, 
		-1, 0, 0, 
		-1, 0, 0, 
		-1, 0, 0, 
		-1, 0, 0, 
		-1, 0, 0, 
		-1, 0, 0, 
		0, -1, 0, 
		0, 1, 0, 
		-0.866025, 0, 0.5, 
		-0.866025, 0, 0.5, 
		-0.866025, 0, 0.5, 
		-0.866025, 0, 0.5, 
		-0.866025, 0, 0.5, 
		-0.866025, 0, 0.5, 
		-0.866025, 0, 0.5, 
		0, -1, 0, 
		0, 1, 0, 
		-0.5, 0, 0.866025, 
		-0.5, 0, 0.866025, 
		-0.5, 0, 0.866025, 
		-0.5, 0, 0.866025, 
		-0.5, 0, 0.866025, 
		-0.5, 0, 0.866025, 
		-0.5, 0, 0.866025, 
		0, -1, 0, 
		0, 1, 0, 
		0, 0, 1, 
		0, 0, 1, 
		0, 0, 1, 
		0, 0, 1, 
		0, 0, 1, 
		0, 0, 1, 
		0, 0, 1, 
		0, -1, 0, 
		0, 1, 0, 
		0.5, 0, 0.866025, 
		0.5, 0, 0.866025, 
		0.5, 0, 0.866025, 
		0.5, 0, 0.866025, 
		0.5, 0, 0.866025, 
		0.5, 0, 0.866025, 
		0.5, 0, 0.866025, 
		0, -1, 0, 
		0, 1, 0, 
		0.866025, 0, 0.5, 
		0.866025, 0, 0.5, 
		0.866025, 0, 0.5, 
		0.866025, 0, 0.5, 
		0.866025, 0, 0.5, 
		0.866025, 0, 0.5, 
		0.866025, 0, 0.5, 
		0.965926, 0, 0.258819, 
		0.965926, 0, 0.258819, 
		0.965926, 0, 0.258819, 
		0.965926, 0, 0.258819, 
		0.965926, 0, 0.258819, 
		0.965926, 0, 0.258819, 
		0.965926, 0, 0.258819
	};

	f32 CylinderTextureMap[] =
	{
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 1,
		0.083333, 0.833333,
		0, 0.833333,
		0.083333, 1,
		0, 0.666667,
		0.083333, 0.666667,
		0.083333, 0.5,
		0, 0.5,
		0, 0.333333,
		0.083333, 0.333333,
		0.083333, 0.166667,
		0, 0.166667,
		0, 0,
		0.083333, 0,
		0, 0,
		0, 0,
		0.166667, 1,
		0.166667, 0.833333,
		0.166667, 0.666667,
		0.166667, 0.5,
		0.166667, 0.333333,
		0.166667, 0.166667,
		0.166667, 0,
		0, 0,
		0, 0,
		0.25, 0.833333,
		0.25, 1,
		0.25, 0.666667,
		0.25, 0.5,
		0.25, 0.333333,
		0.25, 0.166667,
		0.25, 0,
		0, 0,
		0, 0,
		0.333333, 1,
		0.333333, 0.833333,
		0.333333, 0.666667,
		0.333333, 0.5,
		0.333333, 0.333333,
		0.333333, 0.166667,
		0.333333, 0,
		0, 0,
		0, 0,
		0.416667, 0.833333,
		0.416667, 1,
		0.416667, 0.666667,
		0.416667, 0.5,
		0.416667, 0.333333,
		0.416667, 0.166667,
		0.416667, 0,
		0, 0,
		0, 0,
		0.5, 1,
		0.5, 0.833333,
		0.5, 0.666667,
		0.5, 0.5,
		0.5, 0.333333,
		0.5, 0.166667,
		0.5, 0,
		0, 0,
		0, 0,
		0.583333, 0.833333,
		0.583333, 1,
		0.583333, 0.666667,
		0.583333, 0.5,
		0.583333, 0.333333,
		0.583333, 0.166667,
		0.583333, 0,
		0, 0,
		0, 0,
		0.666667, 1,
		0.666667, 0.833333,
		0.666667, 0.666667,
		0.666667, 0.5,
		0.666667, 0.333333,
		0.666667, 0.166667,
		0.666667, 0,
		0, 0,
		0, 0,
		0.75, 0.833333,
		0.75, 1,
		0.75, 0.666667,
		0.75, 0.5,
		0.75, 0.333333,
		0.75, 0.166667,
		0.75, 0,
		0, 0,
		0, 0,
		0.833333, 1,
		0.833333, 0.833333,
		0.833333, 0.666667,
		0.833333, 0.5,
		0.833333, 0.333333,
		0.833333, 0.166667,
		0.833333, 0,
		0, 0,
		0, 0,
		0.916667, 0.833333,
		0.916667, 1,
		0.916667, 0.666667,
		0.916667, 0.5,
		0.916667, 0.333333,
		0.916667, 0.166667,
		0.916667, 0,
		1, 1,
		1, 0.833333,
		1, 0.666667,
		1, 0.5,
		1, 0.333333,
		1, 0.166667,
		1, 0
	};

	s32 CylinderIndices[] = 
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 6, 9, 7, 8, 7, 10, 10, 7, 11, 10, 12, 13, 10, 11, 12, 13, 12, 14, 14, 12, 15, 14, 16, 17, 14, 15, 16, 17, 16, 18, 18, 16, 19, 2, 1, 20, 4, 21, 5, 9, 22, 7, 7, 22, 23, 7, 24, 11, 7, 23, 24, 11, 24, 12, 12, 24, 25, 12, 26, 15, 12, 25, 26, 15, 26, 16, 16, 26, 27, 16, 28, 19, 16, 27, 28, 20, 1, 29, 21, 30, 5, 22, 31, 23, 22, 32, 31, 23, 31, 24, 24, 31, 33, 24, 34, 25, 24, 33, 34, 25, 34, 26, 26, 34, 35, 26, 36, 27, 26, 35, 36, 27, 36, 28, 28, 36, 37, 29, 1, 38, 30, 39, 5, 32, 40, 31, 31, 40, 41, 31, 42, 33, 31, 41, 42, 33, 42, 34, 34, 42, 43, 34, 44, 35, 34, 43, 44, 35, 44, 36, 36, 44, 45, 36, 46, 37, 36, 45, 46, 38, 1, 47, 39, 48, 5, 40, 49, 41, 40, 50, 49, 41, 49, 42, 42, 49, 51, 42, 52, 43, 42, 51, 52, 43, 52, 44, 44, 52, 53, 44, 54, 45, 44, 53, 54, 45, 54, 46, 46, 54, 55, 47, 1, 56, 48, 57, 5, 50, 58, 49, 49, 58, 59, 49, 60, 51, 49, 59, 60, 51, 60, 52, 52, 60, 61, 52, 62, 53, 52, 61, 62, 53, 62, 54, 54, 62, 63, 54, 64, 55, 54, 63, 64, 56, 1, 65, 57, 66, 5, 58, 67, 59, 58, 68, 67, 59, 67, 60, 60, 67, 69, 60, 70, 61, 60, 69, 70, 61, 70, 62, 62, 70, 71, 62, 72, 63, 62, 71, 72, 63, 72, 64, 64, 72, 73, 65, 1, 74, 66, 75, 5, 68, 76, 67, 67, 76, 77, 67, 78, 69, 67, 77, 78, 69, 78, 70, 70, 78, 79, 70, 80, 71, 70, 79, 80, 71, 80, 72, 72, 80, 81, 72, 82, 73, 72, 81, 82, 74, 1, 83, 75, 84, 5, 76, 85, 77, 76, 86, 85, 77, 85, 78, 78, 85, 87, 78, 88, 79, 78, 87, 88, 79, 88, 80, 80, 88, 89, 80, 90, 81, 80, 89, 90, 81, 90, 82, 82, 90, 91, 83, 1, 92, 84, 93, 5, 86, 94, 85, 85, 94, 95, 85, 96, 87, 85, 95, 96, 87, 96, 88, 88, 96, 97, 88, 98, 89, 88, 97, 98, 89, 98, 90, 90, 98, 99, 90, 100, 91, 90, 99, 100, 92, 1, 101, 93, 102, 5, 94, 103, 95, 94, 104, 103, 95, 103, 96, 96, 103, 105, 96, 106, 97, 96, 105, 106, 97, 106, 98, 98, 106, 107, 98, 108, 99, 98, 107, 108, 99, 108, 100, 100, 108, 109, 101, 1, 0, 102, 3, 5, 104, 110, 103, 103, 110, 111, 103, 112, 105, 103, 111, 112, 105, 112, 106, 106, 112, 113, 106, 114, 107, 106, 113, 114, 107, 114, 108, 108, 114, 115, 108, 116, 109, 108, 115, 116
	};

//! constructor
CCapsuleSceneNode::CCapsuleSceneNode(f32 height, f32 radius, ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position, const core::vector3df& rotation,	const core::vector3df& scale)
: ISceneNode(parent, mgr, id, position, rotation, scale), Height(height), Radius(radius)
{
	#ifdef _DEBUG
	setDebugName("CCapsuleSceneNode");
	#endif


	setHeightAndRadius();
}


//! destructor
CCapsuleSceneNode::~CCapsuleSceneNode()
{
}


void CCapsuleSceneNode::setHeightAndRadius()
{
	video::SColor clr(255,255,255,255);
	f32 radius = Radius * 2.0f;
	f32 height = Height;
	s32 i, j, sizeVertices, sizeIndices, index;
	////////////////////////ADD CYLINDER////////////////////////
	sizeVertices = sizeof(CylinderVertices) / sizeof(f32);
	sizeIndices = sizeof(CylinderIndices) / sizeof(s32);
	Buffer[0].Vertices.set_used(sizeVertices);
	Buffer[0].Indices.set_used(sizeIndices);
	i = 0;
	j = 0;
	for(index = 0; index < sizeVertices; index++)
	{
		Buffer[0].Vertices[index] = video::S3DVertex(CylinderVertices[i+0], CylinderVertices[i+1], CylinderVertices[i+2], CylinderNormals[i+0], CylinderNormals[i+1], CylinderNormals[i+2], clr, CylinderTextureMap[j+0], CylinderTextureMap[j+1]); 
		i += 3;
		j += 2;
		Buffer[0].Vertices[index].Pos *= core::vector3df(radius, height, radius);
	}
	for(index = 0; index < sizeIndices; index++)
	{
		Buffer[0].Indices[index] = CylinderIndices[index];
	}
	////////////////////////ADD SPHERE 1////////////////////////
	sizeVertices = sizeof(SphereVertices) / sizeof(f32);
	sizeIndices = sizeof(SphereIndices) / sizeof(s32);
	Buffer[1].Vertices.set_used(sizeVertices);
	Buffer[1].Indices.set_used(sizeIndices);
	i = 0;
	j = 0;
	for(index = 0; index < sizeVertices; index++)
	{
		Buffer[1].Vertices[index]  = video::S3DVertex(SphereVertices[i+0], SphereVertices[i+1], SphereVertices[i+2], SphereNormals[i+0], SphereNormals[i+1], SphereNormals[i+2], clr, SphereTextureMap[j+0], SphereTextureMap[j+1]); 
		i += 3;
		j += 2;
		Buffer[1].Vertices[index].Pos *= core::vector3df(radius, radius, radius);
		Buffer[1].Vertices[index].Pos += core::vector3df(0, -height * 0.5f, 0);
	}
	for(index = 0; index < sizeIndices; index++)
	{
		Buffer[1].Indices[index] = SphereIndices[index];
	}
	////////////////////////ADD SPHERE 2////////////////////////
	sizeVertices = sizeof(SphereVertices) / sizeof(f32);
	sizeIndices = sizeof(SphereIndices) / sizeof(s32);
	Buffer[2].Vertices.set_used(sizeVertices);
	Buffer[2].Indices.set_used(sizeIndices);
	i = 0;
	j = 0;
	for(index = 0; index < sizeVertices; index++)
	{
		Buffer[2].Vertices[index]  = video::S3DVertex(SphereVertices[i+0], SphereVertices[i+1], SphereVertices[i+2], SphereNormals[i+0], SphereNormals[i+1], SphereNormals[i+2], clr, SphereTextureMap[j+0], SphereTextureMap[j+1]); 
		i += 3;
		j += 2;
		Buffer[2].Vertices[index].Pos *= core::vector3df(radius, radius, radius);
		Buffer[2].Vertices[index].Pos += core::vector3df(0, height * 0.5f, 0);
	}
	for(index = 0; index < sizeIndices; index++)
	{
		Buffer[2].Indices[index] = SphereIndices[index];
	}

	Buffer[0].BoundingBox.reset(0,0,0);
	Buffer[0].BoundingBox.addInternalPoint(radius * 0.5f,0.0f,0.0f);
	Buffer[0].BoundingBox.addInternalPoint(-radius * 0.5f,0.0f,0.0f);
	Buffer[0].BoundingBox.addInternalPoint(0.0f,0.0f,radius * 0.5f);
	Buffer[0].BoundingBox.addInternalPoint(0.0f,0.0f,-radius * 0.5f);
	Buffer[0].BoundingBox.addInternalPoint(0.0f,Height*0.5f,0.0f);
	Buffer[0].BoundingBox.addInternalPoint(0.0f,-Height*0.5f,0.0f);

	radius *= 0.5f;

	Buffer[1].BoundingBox.reset(0.0f,-radius - height * 0.5f,0.0f);
	Buffer[1].BoundingBox.addInternalPoint(0.0f,radius - height * 0.5f,0.0f);
	Buffer[1].BoundingBox.addInternalPoint(radius,0.0f - height * 0.5f,0.0f);
	Buffer[1].BoundingBox.addInternalPoint(-radius,0.0f - height * 0.5f,0.0f);
	Buffer[1].BoundingBox.addInternalPoint(0.0f,0.0f - height * 0.5f,radius);
	Buffer[1].BoundingBox.addInternalPoint(0.0f,0.0f - height * 0.5f,-radius);
	
	Buffer[2].BoundingBox.reset(0.0f,-radius + height * 0.5f,0.0f);
	Buffer[2].BoundingBox.addInternalPoint(0.0f,radius + height * 0.5f,0.0f);
	Buffer[2].BoundingBox.addInternalPoint(radius,0.0f + height * 0.5f,0.0f);
	Buffer[2].BoundingBox.addInternalPoint(-radius,0.0f + height * 0.5f,0.0f);
	Buffer[2].BoundingBox.addInternalPoint(0.0f,0.0f + height * 0.5f,radius);
	Buffer[2].BoundingBox.addInternalPoint(0.0f,0.0f + height * 0.5f,-radius);

	BBox.addInternalBox(Buffer[0].BoundingBox);
	BBox.addInternalBox(Buffer[1].BoundingBox);
	BBox.addInternalBox(Buffer[2].BoundingBox);

}



//! renders the node.
void CCapsuleSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (Buffer[0].Vertices.size() && Buffer[0].Indices.size() && Buffer[1].Vertices.size() && Buffer[1].Indices.size() && Buffer[2].Vertices.size() && Buffer[2].Indices.size())
	{
		driver->setMaterial(Buffer[0].Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawMeshBuffer(&Buffer[0]);
		driver->drawMeshBuffer(&Buffer[1]);
		driver->drawMeshBuffer(&Buffer[2]);
		if (DebugDataVisible)
		{
			video::SMaterial m;
			m.Lighting = false;
			driver->setMaterial(m);
			driver->draw3DBox(BBox, video::SColor(255,255,255,255));
		}
	}
}



//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CCapsuleSceneNode::getBoundingBox() const
{
	return BBox;
}


void CCapsuleSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& CCapsuleSceneNode::getMaterial(u32 i)
{
	return Buffer[0].Material;
}


//! returns amount of materials used by this scene node.
u32 CCapsuleSceneNode::getMaterialCount()
{
	return 1;
}


//! Writes attributes of the scene node.
void CCapsuleSceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options)
{
	ISceneNode::serializeAttributes(out, options);

	out->addFloat("Height", Height);
	out->addFloat("Radius", Radius);
}


//! Reads attributes of the scene node.
void CCapsuleSceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	Height = in->getAttributeAsFloat("Height");
	Height = irr::core::max_(Height, 0.0001f);
	Radius = in->getAttributeAsFloat("Radius");
	Radius = irr::core::max_(Radius, 0.0001f);

	setHeightAndRadius();

	ISceneNode::deserializeAttributes(in, options);
}



} // end namespace scene
} // end namespace irr
