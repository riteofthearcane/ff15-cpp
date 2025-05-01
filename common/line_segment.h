class LineSegment
{
public:
    std::vector<SDK::Structs::Math::Vector3> points; // Stores the two endpoints of the segment.

    LineSegment(const SDK::Structs::Math::Vector3 &a = SDK::Structs::Math::Vector3(), const SDK::Structs::Math::Vector3 &b = SDK::Structs::Math::Vector3())
    {
        points.push_back(a);
        points.push_back(b);
    }

    // Equality operator to compare two line segments.
    bool operator==(const LineSegment &other)
    {
        return (points[0] == other.points[0] && points[1] == other.points[1]) ||
               (points[1] == other.points[0] && points[0] == other.points[1]);
    }

    // Retrieve the segment as part of a collection.
    std::vector<LineSegment> GetLineSegments() const
    {
        std::vector<LineSegment> segments;
        segments.push_back(*this);
        return segments;
    }

    // Calculate the direction vector of the segment.
    SDK::Structs::Math::Vector3 Direction()
    {
        return points[1] - points[0];
    }

    // Get the length of the segment.
    float Length()
    {
        return (points[1] - points[0]).len();
    }

    // Check if a point lies on one of the segment's endpoints.
    bool Contains(SDK::Structs::Math::Vector3 &spatialObject)
    {
        return spatialObject.Distance2D(points[0]) < 0.01f || spatialObject.Distance2D(points[1]) < 0.01f;
    }

    // Check if a point is within a given distance (width) to the line segment
    bool IsWithinLineSegment(const LineSegment &line, const SDK::Structs::Math::Vector3 &point, float width) const
    {
        float distance = line.DistanceTo(point);
        return distance <= width;
    }

    // Calculate the distance from a point to the line segment.
    float DistanceTo(const SDK::Structs::Math::Vector3 &spatialObject) const
    {
        SDK::Structs::Math::Vector3 pt = {spatialObject.x, 0, spatialObject.z};
        SDK::Structs::Math::Vector3 p1 = {points[0].x, 0, points[0].z};
        SDK::Structs::Math::Vector3 p2 = {points[1].x, 0, points[1].z};

        float dx = p2.x - p1.x;
        float dz = p2.z - p1.z;

        if (dx == 0 && dz == 0)
        {
            return pt.Distance2D(p1);
        }

        float t = ((pt.x - p1.x) * dx + (pt.z - p1.z) * dz) / (dx * dx + dz * dz);

        if (t < 0)
        {
            return pt.Distance2D(p1);
        }
        else if (t > 1)
        {
            return pt.Distance2D(p2);
        }
        else
        {
            SDK::Structs::Math::Vector3 closest = {p1.x + t * dx, 0, p1.z + t * dz};
            return pt.Distance2D(closest);
        }
    }

    // Check if the segment intersects another segment.
    bool Intersects(const LineSegment &other) const
    {
        float L1_X1 = points[0].x, L1_Y1 = points[0].z, L1_X2 = points[1].x, L1_Y2 = points[1].z;
        float L2_X1 = other.points[0].x, L2_Y1 = other.points[0].z, L2_X2 = other.points[1].x, L2_Y2 = other.points[1].z;

        float d = (L2_Y2 - L2_Y1) * (L1_X2 - L1_X1) - (L2_X2 - L2_X1) * (L1_Y2 - L1_Y1);

        if (d == 0)
        {
            return false; // Lines are parallel
        }

        float n_a = (L2_X2 - L2_X1) * (L1_Y1 - L2_Y1) - (L2_Y2 - L2_Y1) * (L1_X1 - L2_X1);
        float n_b = (L1_X2 - L1_X1) * (L1_Y1 - L2_Y1) - (L1_Y2 - L1_Y1) * (L1_X1 - L2_X1);

        float ua = n_a / d;
        float ub = n_b / d;

        return (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1);
    }

    // Get the closest point on the segment to a given point.
    SDK::Structs::Math::Vector3 Closest(const SDK::Structs::Math::Vector3 &v) const
    {
        float dx = points[1].x - points[0].x;
        float dz = points[1].z - points[0].z;

        if (dx == 0 && dz == 0)
        {
            return points[0]; // Points are the same
        }

        float t = ((v.x - points[0].x) * dx + (v.z - points[0].z) * dz) / (dx * dx + dz * dz);

        if (t < 0)
        {
            return points[0];
        }
        else if (t > 1)
        {
            return points[1];
        }
        else
        {
            return SDK::Structs::Math::Vector3(points[0].x + t * dx, 0, points[0].z + t * dz);
        }
    }
};
