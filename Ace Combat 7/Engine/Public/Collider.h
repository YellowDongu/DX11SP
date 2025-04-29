#pragma once
#include "Component.h"

namespace Engine
{
	enum class ColliderType
	{
		  AABB
		, OBB
		, Sphere
		, Frustum
		, End
	};


	struct ENGINEDLL Bounder abstract
	{
		struct Description
		{
			Vector3 center = Vector3::zero();
			ColliderType colliderType = ColliderType::End;
		};
		ColliderType colliderType;

		virtual void DebugRender(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, float4 color) = 0;
		virtual void Update(const Matrix& worldMatrix) = 0;
		virtual bool Intersect(Bounder* other, Vector3& direction, float& amount) = 0;
		virtual Bounder* Clone(void) = 0;
		virtual float GetInterectRadius(void) = 0;
		virtual Vector3 GetWorldCenter(void) = 0;
	};

	struct ENGINEDLL AABB : public Bounder
	{
		AABB(void) { colliderType = ColliderType::AABB; }
		struct Description : public Bounder::Description
		{
			float3 extents;
			Description(void) {colliderType = ColliderType::AABB; }
		};
		void SetData(const Description& infomation)
		{
			bounder.Center = infomation.center;
			bounder.Extents = infomation.extents;
			localBounder = bounder;
		}
		static AABB* Create(const Description& infomation)
		{
			AABB* newInstance = new AABB();
			newInstance->SetData(infomation);
			return newInstance;
		}
		virtual void DebugRender(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, float4 color)
		{
			batch->Begin();
			DX::Draw(batch, bounder, DirectX::XMLoadFloat4(&color));
			batch->End();
		}
		virtual void Update(const Matrix& worldMatrix)
		{
			xmMatrix TransformMatrix = DirectX::XMLoadFloat4x4(&worldMatrix);
			TransformMatrix.r[0] = DirectX::XMVectorMultiply(DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f), DirectX::XMVector3Length(TransformMatrix.r[0]));
			TransformMatrix.r[1] = DirectX::XMVectorMultiply(DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f), DirectX::XMVector3Length(TransformMatrix.r[1]));
			TransformMatrix.r[2] = DirectX::XMVectorMultiply(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), DirectX::XMVector3Length(TransformMatrix.r[2]));

			localBounder.Transform(bounder, TransformMatrix);
		}
		virtual bool Intersect(Bounder* other, Vector3& direction, float& amount);
		virtual Bounder* Clone(void) override
		{
			return new AABB(*this);
		}
		virtual float GetInterectRadius(void) override { return sqrtf(bounder.Extents.x * bounder.Extents.x + bounder.Extents.y * bounder.Extents.y + bounder.Extents.z * bounder.Extents.z); }
		virtual Vector3 GetWorldCenter(void) { return bounder.Center; }

		DirectX::BoundingBox bounder;
		DirectX::BoundingBox localBounder;
	};

	struct ENGINEDLL OBB : public Bounder
	{
		struct Description : public Bounder::Description
		{
			float3 extents;
			float4 quaternion;
			Description(void) : extents(Vector3::zero()), quaternion{} { colliderType = ColliderType::OBB; }
		};
		void SetData(const Description& infomation)
		{
			bounder.Center = infomation.center;
			bounder.Extents = infomation.extents;
			bounder.Orientation = infomation.quaternion;
			localBounder = bounder;
		}
		OBB(void) { colliderType = ColliderType::OBB; }
		static OBB* Create(const Description& infomation)
		{
			OBB* newInstance = new OBB();
			newInstance->SetData(infomation);
			return newInstance;
		}
		virtual void DebugRender(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, float4 color)
		{
			batch->Begin();
			DX::Draw(batch, bounder, DirectX::XMLoadFloat4(&color));
			batch->End();
		}
		virtual void Update(const Matrix& worldMatrix)
		{
			localBounder.Transform(bounder, DirectX::XMLoadFloat4x4(&worldMatrix));
		}
		virtual bool Intersect(Bounder* other, Vector3& direction, float& amount);
		virtual Bounder* Clone(void) override
		{
			return new OBB(*this);
		}
		virtual float GetInterectRadius(void) override { return sqrtf(bounder.Extents.x * bounder.Extents.x + bounder.Extents.y * bounder.Extents.y + bounder.Extents.z * bounder.Extents.z); }
		virtual Vector3 GetWorldCenter(void) { return bounder.Center; }


		DirectX::BoundingOrientedBox bounder;
		DirectX::BoundingOrientedBox localBounder;
	};

	struct ENGINEDLL Sphere : public Bounder
	{
		Sphere(void) { colliderType = ColliderType::Sphere; }
		struct Description : public Bounder::Description
		{
			float radius;
			Description(void) : radius(0.0f) { colliderType = ColliderType::Sphere; }
		};
		void SetData(const Description& infomation)
		{
			bounder.Center = infomation.center;
			bounder.Radius = infomation.radius;
			localBounder = bounder;
		}
		static Sphere* Create(const Description& infomation)
		{
			Sphere* newInstance = new Sphere();
			newInstance->SetData(infomation);
			return newInstance;
		}
		virtual void DebugRender(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, float4 color)
		{
			batch->Begin();
			DX::Draw(batch, bounder, DirectX::XMLoadFloat4(&color));
			batch->End();
		}
		virtual void Update(const Matrix& worldMatrix)
		{
			localBounder.Transform(bounder, DirectX::XMLoadFloat4x4(&worldMatrix));
		}
		virtual bool Intersect(Bounder* other, Vector3& direction, float& amount);
		virtual Bounder* Clone(void) override
		{
			return new Sphere(*this);
		}
		virtual float GetInterectRadius(void) override { return bounder.Radius; }
		virtual Vector3 GetWorldCenter(void) { return bounder.Center; }


		DirectX::BoundingSphere bounder;
		DirectX::BoundingSphere localBounder;
	};

	struct ENGINEDLL Frustum : public Bounder
	{
		Frustum(void) { colliderType = ColliderType::Frustum; }
		struct Description : public Bounder::Description
		{
			float4 Orientation;

			float RightSlope;
			float LeftSlope;
			float TopSlope;
			float BottomSlope;
			float Near, Far;
			Description(void) { colliderType = ColliderType::Frustum; }
		};
		void SetData(const Description& infomation)
		{
			bounder.Orientation = infomation.Orientation;
			bounder.RightSlope = infomation.RightSlope;
			bounder.LeftSlope = infomation.LeftSlope;
			bounder.TopSlope = infomation.TopSlope;
			bounder.BottomSlope = infomation.BottomSlope;
			bounder.Near = infomation.Near;
			bounder.Far = infomation.Far;
			localBounder = bounder;
		}
		static Frustum* Create(const Description& infomation)
		{
			return nullptr;
		}
		virtual void DebugRender(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, float4 color)
		{
			batch->Begin();
			DX::Draw(batch, bounder, DirectX::XMLoadFloat4(&color));
			batch->End();
		}
		virtual void Update(const Matrix& worldMatrix)
		{
			localBounder.Transform(bounder, DirectX::XMLoadFloat4x4(&worldMatrix));
		}
		virtual bool Intersect(Bounder* other, Vector3& direction, float& amount)
		{
			return false;
		}
		virtual Bounder* Clone(void) override
		{
			return new Frustum(*this);
		}
		virtual float GetInterectRadius(void) override
		{
			float maxDistanceSqare = 0.0f, distanceSqare = 0.0f;
			DirectX::XMFLOAT3 corners[8];
			bounder.GetCorners(corners);

			for (int i = 0; i < 8; ++i)
			{
				for (int j = i + 1; j < 8; ++j)
				{
					distanceSqare = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&corners[i]), DirectX::XMLoadFloat3(&corners[j]))));
					if (distanceSqare > maxDistanceSqare) maxDistanceSqare = distanceSqare;
				}
			}

			return sqrtf(maxDistanceSqare) * 0.5f; // 반지름 추정
		}
		virtual Vector3 GetWorldCenter(void) { return bounder.Origin; }

		DirectX::BoundingFrustum bounder;
		DirectX::BoundingFrustum localBounder;
	};

	#ifndef ColliderDebug
	#define ColliderDebug
	#endif

	class ENGINEDLL Collider : public Component
	{
	private:
		Collider(void) = delete;
		Collider(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext);
		Collider(const Collider& other);
		virtual ~Collider(void) = default;
		virtual void Free(void) override;
	public:
		static Collider* Create(ID3D11Device* dxDevice, ID3D11DeviceContext* dxDeviceContext, Bounder::Description* description);
		Component* Clone(void);

		HRESULT Start(void);
		void Update(void);
		void LateUpdate(void);
		void FixedUpdate(void);
		void Render(void);

		void SetException(Engine::GameObject* object) { exception.push_back(object); }
		HRESULT CreateCollider(Bounder::Description* description);
		bool Intersect(Collider* other);
	private:
		bool clone{false}, updated;
		Bounder* collidingBody{nullptr};
		ID3D11Device* device{nullptr};
		ID3D11DeviceContext* context{nullptr};
		std::vector<Engine::GameObject*> exception;

		#ifdef ColliderDebug
		DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch{ nullptr };
		DirectX::BasicEffect* effect{ nullptr };
		ID3D11InputLayout* inputLayout = { nullptr };
		#endif
	};
}