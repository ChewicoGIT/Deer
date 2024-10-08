class Vec3
{
    float x, y, z;
    
    Vec3(float _x = 0, float _y = 0, float _z = 0) {
        x = _x;
        y = _y;
        z = _z;
    }

    Vec3 opAdd(const Vec3 &in other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 opSub(const Vec3 &in other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 opMul(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    bool opEquals(const Vec3 &in other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    Vec3 opNeg() const {
        return Vec3(-x, -y, -z);
    }

	void normalize() {
		float magnitude = getMagnitude();
		if (magnitude > 0) {
			float factor = 1 / magnitude;

			x = x * factor;
			y = y * factor;
			z = z * factor;
		}
	}

	float getMagnitude() {
		return sqrt(x * x + y * y + z * z);
	}

	string opImplConv() const {
        return "(" + x + ", " + y + ", " + z + ")";
    }
}
