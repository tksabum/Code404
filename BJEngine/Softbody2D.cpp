#include "Softbody2D.h"

#include "ConvexpolygonCollider.h"
#include "GameObject.h"
#include "BJMath.h"

using namespace std;

namespace BJEngine
{
	using namespace Math;

	const float Softbody2D::MASS_DEFAULT = 100.0f;
	const float Softbody2D::MASS_MIN = 0.000001f;
	const float Softbody2D::RESTITUTION_VALUE_DEFAULT = 1.0f;
	const float Softbody2D::RESTITUTION_VALUE_MIN = 0.0f;
	const float Softbody2D::RESTITUTION_VALUE_MAX = 1.0f;
	const Vector2D Softbody2D::GRAVITY_DEFAULT = Vector2D(0.0f, 10.0f);

	const int Softbody2D::DEPTH_DEFAULT = 5;
	const float Softbody2D::SOFTNESS_FACTOR_DEFAULT = 0.3f;

	Softbody2D::Softbody2D() :
		velocity(Vector2D(0.0f, 0.0f)),
		rotationalVelocity(0.0f),
		mass(MASS_DEFAULT),
		restitutionValue(RESTITUTION_VALUE_DEFAULT),
		affectedByGravity(true),
		positionXLocked(false),
		positionYLocked(false),
		rotationLocked(false),
		gravity(GRAVITY_DEFAULT),
		depth(DEPTH_DEFAULT),
		softnessFactor(SOFTNESS_FACTOR_DEFAULT),
		vertexCount(0)
	{
	}

	Softbody2D::~Softbody2D()
	{

	}

	void Softbody2D::Awake()
	{
		collider = GetGameObject()->GetComponent<ConvexpolygonCollider>();

		if (collider != nullptr)
		{
			vector<Vector2D> outermostvertex = collider->GetPoints();
			vertexCount = outermostvertex.size();

			orgNodes.resize(depth, vector<Vector2D>(vertexCount));

			orgVerticalSpring.resize(depth, vector<float>(vertexCount));
			orgHorizontalSpring.resize(depth, vector<float>(vertexCount));

			verticalSpringRepulsiveForce.resize(depth, vector<float>(vertexCount));
			horizontalSpringRepulsiveForce.resize(depth, vector<float>(vertexCount));

			for (int i = 0; i < vertexCount; i++)
			{
				orgNodes[0][i] = outermostvertex[i];
			}

			for (int i = 1; i < depth; i++)
			{
				for (int j = 0; j < vertexCount; j++)
				{
					Vector2D parentpoint1 = orgNodes[i - 1][j];
					Vector2D parentpoint2 = orgNodes[i - 1][(j + 1) % vertexCount];

					Vector2D middlepoint = (parentpoint1 + parentpoint2) / 2.0f;
					Vector2D middleunitvector = middlepoint.GetUnitVector();

					float costheta = DotProduct(middleunitvector, parentpoint2.GetUnitVector());
					float sintheta = sqrt(1.0f - costheta * costheta);

					Vector2D currentdepthnode = middleunitvector * (parentpoint2.Size() / (2.0f * sintheta));

					orgNodes[i][j] = currentdepthnode;
				}
			}

			for (int i = 0; i < depth; i++)
			{
				for (int j = 0; j < 2 * vertexCount; j++)
				{
					int parentidx = j / 2;

					// 하다말았음
				}
			}


		}
	}
}

