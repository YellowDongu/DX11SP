#pragma once

#ifdef _WINDOWS

//typedef unsigned char	ubyte;
//typedef signed char		byte;
typedef char			CHAR;
typedef	wchar_t			WCHAR;

//typedef	bool			BOOL;

typedef short			SHORT;
typedef unsigned short	USHORT;

typedef int				INT;
typedef unsigned int	UINT;

typedef long			LONG;
typedef unsigned long	ULONG;

typedef float			FLOAT;
typedef double			DOUBLE;


typedef std::string		string;
typedef std::wstring	wstring;



// 메모장용
typedef DirectX::XMVECTOR xmVector;
typedef DirectX::FXMVECTOR fxmvec, fxmVector;
typedef DirectX::GXMVECTOR gxmvec, gxmVector;
typedef DirectX::HXMVECTOR hxmvec, hxmVector;
typedef DirectX::CXMVECTOR cxmvec, cxmVector;

typedef DirectX::XMMATRIX xmMatrix;
typedef DirectX::FXMMATRIX fxmmat, fxmMatrix;
typedef DirectX::CXMMATRIX cxmmat, cxmMatrix;
//



//typedef DirectX::XMFLOAT2 float2;
//typedef DirectX::XMFLOAT3 float3;
typedef DirectX::XMFLOAT4 float4;

typedef DirectX::XMFLOAT3X3 float3x3;
typedef DirectX::XMFLOAT4X4 float4x4, Matrix;

typedef struct Vector2 : DirectX::XMFLOAT2
{
public:
	Vector2(void) : DirectX::XMFLOAT2(0.0f, 0.0f) {}
	Vector2(float x, float y) : DirectX::XMFLOAT2(x, y) {}
	Vector2(const DirectX::XMFLOAT2& vector) : DirectX::XMFLOAT2(vector) {}

	operator DirectX::XMFLOAT2() const { return DirectX::XMFLOAT2(x, y); };

	bool operator==(const Vector2& other) const
	{
		return (x == other.x) && (y == other.y);
	}
	bool operator!=(const Vector2& other) const
	{
		return (x != other.x) || (y != other.y);
	}

	Vector2 operator+(const Vector2& other) const
	{
		return { x + other.x, y + other.y };
	}
	Vector2 operator-(const Vector2& other) const
	{
		return { x - other.x, y - other.y };
	}
	Vector2 operator*(const float& i) const
	{
		return { x * i, y * i };
	}
	Vector2 operator/(const int& i) const
	{
		return { x / i, y / i };
	}

	void operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
	}
	void operator*=(const float& i)
	{
		x *= i;
		y *= i;
	}
	void operator/=(const float& i)
	{
		x /= i;
		y /= i;
	}
	void operator=(const Vector2& other)
	{
		x = other.x;
		y = other.y;
	}


	//전위연산
	Vector2& operator++()
	{
		x++;
		y++;
		return *this;
	}
	// 후위연산
	Vector2 operator++(int)
	{
		Vector2 temp{ x, y };
		x++;
		y++;
		return temp;
	}

	Vector2& operator--()
	{
		x--;
		y--;
		return *this;
	}
	Vector2 operator--(int)
	{
		Vector2 temp{ x, y };
		x--;
		y--;
		return temp;
	}

	// 좌표 절대값
	Vector2 absolute(void) const
	{
		return { abs(x) ,abs(y) };
	}
	static Vector2 absolute(const Vector2& obj)
	{
		return { abs(obj.x) ,abs(obj.y) };
	}

	// 벡터 내적
	float dot(const Vector2& other) const
	{
		return x * other.x + y * other.y;
	}
	static float dot(const Vector2& obj, const Vector2& sbj)
	{
		return obj.x * sbj.x + obj.y * sbj.y;
	}

	// 벡터의 크기(스칼라)
	float magnitude(void) const
	{
		return static_cast<float>(std::sqrt(x * x + y * y));
	}
	static float magnitude(const Vector2& obj)
	{
		return static_cast<float>(std::sqrt(obj.x * obj.x + obj.y * obj.y));
	}

	// 두 좌표 간 상대 거리
	float distance(const Vector2& sbj)
	{
		return (*this - sbj).magnitude();
	}
	static float distance(const Vector2& _Vec1, const Vector2& _Vec2)
	{
		return (_Vec1 - _Vec2).magnitude();
	}

	// 수직 선분을 나타냄
	Vector2 perpendicular(void) const
	{
		return { -y, x };
	}
	static Vector2 perpendicular(const Vector2& obj)
	{
		return { -obj.y, obj.x };
	}

	// 단위 벡터 표현 (숫자 0~1로 범위 고정)
	Vector2 normalize() const
	{
		float length = (*this).magnitude();
		return { x / length, y / length };
	}
	static Vector2 normalize(const Vector2& obj)
	{
		float length = obj.magnitude();
		return { obj.x / length, obj.y / length };
	}

	// 라디안 각도를 단위 벡터로 나타내줌
	static Vector2 getDirectionVector(float _angle)
	{
		return Vector2(std::cos(_angle), std::sin(_angle));
	}

	// 현재 라디안 각도의 전방을 알려줌
	Vector2 forward(float _rotation) const
	{
		Vector2 DirectionVec = getDirectionVector(_rotation);
		return Vector2(x * DirectionVec.x - y * DirectionVec.y, x * DirectionVec.y + y * DirectionVec.x);
	}

	// 벡터를 도 단위 각도로 만들어줌
	static float angle(const Vector2& directionVec)
	{
		Vector2 normalVec = directionVec.normalize();
		float acosAng = acos(normalVec.x) * (180.0f / 3.14159265358979f);
		if (normalVec.y < 0) { acosAng = 360.0f - acosAng; }
		return acosAng;
	}

	// 입사벡터를 선분벡터에 대한 반사벡터로 만들어줌
	static Vector2 reflect(const Vector2& vector, const Vector2& segment)
	{
		Vector2 normalVec = segment.perpendicular().normalize();
		return vector - normalVec * (2 * dot(vector, normalVec));
	}


	static Vector2 zero(void) { return { 0.0f, 0.0f }; }
	static Vector2 one(void) { return { 1.0f, 1.0f }; }

} float2;

typedef struct Vector3 : DirectX::XMFLOAT3
{
	Vector3(void) : DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) {}
	Vector3(FLOAT x, FLOAT y, FLOAT z) : DirectX::XMFLOAT3(x, y, z) {}
	Vector3(DirectX::XMFLOAT3 vector) : DirectX::XMFLOAT3(vector) {}
	virtual ~Vector3(void) {}

	static Vector3 zero(void) { return Vector3{ 0.0f, 0.0f, 0.0f }; }
	static Vector3 one(void) { return Vector3{ 1.0f, 1.0f, 1.0f }; }

	operator DirectX::XMVECTOR(void) { return DirectX::XMLoadFloat3(this); }

	// 연산자 정의
	void operator=(const DirectX::FXMVECTOR other)
	{
		DirectX::XMStoreFloat3(this, other);
	}
	bool operator==(const Vector3& other) const
	{
		return (x == other.x) && (y == other.y) && (z == other.z);
	}
	bool operator!=(const Vector3& other) const
	{
		return (x != other.x) || (y != other.y) || (z != other.z);
	}
	bool operator>(const Vector3& other) const
	{
		return (x > other.x) && (y > other.y) && (z > other.z);
	}
	bool operator<(const Vector3& other) const
	{
		return (x < other.x) && (y < other.y) && (z < other.z);
	}
	bool operator>=(const Vector3& other) const
	{
		return (x >= other.x) && (y >= other.y) && (z >= other.z);
	}
	bool operator<=(const Vector3& other) const
	{
		return (x <= other.x) && (y <= other.y) && (z <= other.z);
	}

	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3 operator*(const float& i) const
	{
		return Vector3(x * i, y * i, z * i);
	}
	Vector3 operator/(const float& i) const
	{
		return Vector3(x / i, y / i, z / i);
	}

	void operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}
	void operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}
	void operator*=(const FLOAT& i)
	{
		x *= i;
		y *= i;
		z *= i;
	}
	void operator/=(const FLOAT& i)
	{
		x /= i;
		y /= i;
		z /= i;
	}



	// 벡터 내적
	float dot(const Vector3& other) const { return x * other.x + y * other.y + z + other.z; }
	static float dot(const Vector3& obj, const Vector3& sbj) { return obj.x * sbj.x + obj.y * sbj.y + obj.z + sbj.z; }

	// 선분의 법선 선분에 대한 반사벡터
	Vector3 reflect(const Vector3& other) const
	{
		float _dot = dot(other); // (R = V - 2 X (V dot N) X N)
		return Vector3(x - 2.0f * _dot * other.x, y - 2.0f * _dot * other.y, z - 2.0f * _dot * other.z);
	}
	static Vector3 reflect(const Vector3& object, const Vector3& subject)
	{
		float _dot = dot(object, subject);
		return Vector3(object.x - 2.0f * _dot * subject.x, object.y - 2.0f * _dot * subject.y, object.z - 2.0f * _dot * subject.z);
	}

	// 스칼라 값
	float magnitude(void) const { return std::sqrt(x * x + y * y + z * z); }
	static float magnitude(const Vector3& obj) { return std::sqrt(obj.x * obj.x + obj.y * obj.y + obj.z * obj.z); }

	// 두 점 사잉의 스칼라 값
	float distance(const Vector3& other) { return (*this - other).magnitude(); }
	static float distance(const Vector3& _Vec1, const Vector3& _Vec2) { return (_Vec1 - _Vec2).magnitude(); }

	// 단위벡터
	Vector3 normalize(void) const
	{
		float length = magnitude();
		return { x / length, y / length, z / length };
	}
	static Vector3 normalize(const Vector3& obj)
	{
		float length = obj.magnitude();
		return { obj.x / length, obj.y / length, obj.z / length };
	}

	// 방위각과 상승각에 대응하는 방향벡터
	static Vector3 direction(FLOAT azimuth, FLOAT elevation)
	{
		// azimuth: 방향각 (0 <= azimuth < 2π) / elevation: 경사각 (-π/2 <= elevation <= π/2)
		while (azimuth >= 0 && azimuth <= 360 && elevation >= 0 && elevation <= 360)
		{
			if (azimuth < 0) { azimuth += 360; }
			if (azimuth > 360) { azimuth -= 360; }
			if (elevation < 0) { elevation += 360; }
			if (elevation > 360) { elevation -= 360; }
		}

		return Vector3(std::cos(elevation) * std::cos(azimuth), std::cos(elevation) * std::sin(azimuth), std::sin(elevation));
	}
	// 방위각에 대응하는 방향벡터
	static Vector3 drection2D(FLOAT azimuth) { return Vector3(std::cos(azimuth), 0.0f, std::sin(azimuth)); }

	// 좌표를 기준으로 방위각과 상승각에 대응하는 방향벡터
	Vector3 forward(FLOAT azimuth, FLOAT elevation) { return *this + direction(azimuth, elevation).normalize(); }
	static Vector3 forward(Vector3 position, FLOAT azimuth, FLOAT elevation) { return position + direction(azimuth, elevation).normalize(); }

	// 벡터 외적
	Vector3 cross(const Vector3& other) const
	{
		return Vector3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}
	static Vector3 cross(const Vector3& obj, const Vector3& sbj)
	{
		return Vector3(
			obj.y * sbj.z - obj.z * sbj.y,
			obj.z * sbj.x - obj.x * sbj.z,
			obj.x * sbj.y - obj.y * sbj.x
		);
	}

	// 벡터 외적
	Vector3 getDirectionRadian(void)
	{
		Vector3 direction = this->normalize();
		return Vector3(asin(-direction.y), atan2(direction.x, direction.z), 0.0f);
	}
	Vector3 getDirection(void)
	{
		Vector3 direction = this->normalize();
		float toDegree = (180.0f / 3.14159265358979f);
		return Vector3(asin(-direction.y) * toDegree, atan2(direction.x, direction.z) * toDegree, 0.0f);
	}
	static Vector3 getDirection(const Vector3& directionVector)
	{
		Vector3 direction = directionVector.normalize();
		float toDegree = (180.0f / 3.14159265358979f);
		return Vector3(asin(-direction.y) * toDegree, atan2(direction.x, direction.z) * toDegree, 0.0f);
	}

	// 3차원 수직벡터(y축)
	Vector3 perpendicular(void) const { return cross(Vector3{ -z, y, x }); }
	static Vector3 perpendicular(const Vector3& obj) { return cross(obj, Vector3(-obj.z, obj.y, obj.x)); }
	// 2차원 수직벡터
	Vector3 perpendicular2D(void) const { return Vector3{ -z, y, x }; }
	static Vector3 perpendicular2D(const Vector3& obj) { return Vector3(-obj.z, obj.y, obj.x); }
} float3;



#endif // _WINDOWS

