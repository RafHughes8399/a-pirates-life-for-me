#ifndef SINGLETON_H
#define SINGLETON_H
#include "../lib/raylib/src/raylib.h"
#include "config.h"

class ObjectType {
public:
    virtual ~ObjectType() = default;

    Model& get_model() { return model_; }
    Texture2D& get_texture() { return texture_; }

protected:
    ObjectType(Model model, Texture2D texture)
        : model_(model), texture_(texture) {
        // Set the texture for the model's first material
    }
 
    Model model_;
    Texture2D texture_;
};

class ShipType : public ObjectType {
public:
    // Delete copy and move operations
    ShipType(const ShipType& other) = delete;
    ShipType(ShipType&& other) = delete;
    ShipType& operator=(const ShipType& other) = delete;
    ShipType& operator=(ShipType&& other) = delete;

    // Singleton getter
    static ShipType& get_instance() {
        static ShipType instance;
        return instance;
    }

private:
    // Private constructor
    ShipType()
        : ObjectType(LoadModel(PIRATE_SHIP_MODEL), LoadTexture(TEXTURE3B)) {
        // Additional initialization if needed
        model_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture_;            // Set map diffuse texture
    }
};

class OceanType : public ObjectType {
public:
    // Delete copy and move operations
    OceanType(const OceanType& other) = delete;
    OceanType(OceanType&& other) = delete;
    OceanType& operator=(const OceanType& other) = delete;
    OceanType& operator=(OceanType&& other) = delete;

    // Singleton getter
    static OceanType& get_instance() {
        static OceanType instance;
        return instance;
    }

private:
    // Private constructor
    OceanType()
        : ObjectType(LoadModelFromMesh(GenMeshCube(WORLD_X, WORLD_Y * 0.5, WORLD_Z)),
            LoadTexture(WATER_TEXTURE)) {
        model_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture_;            // Set map diffuse texture
        // Additional initialization if needed
    }
};

class HubType : public ObjectType {
public:
    HubType(const HubType& other) = delete;
    HubType(HubType&& other) = delete;
    HubType& operator=(const HubType& other) = delete;
    HubType& operator=(HubType&& other) = delete;

    // Singleton getter
    static HubType& get_instance() {
        static HubType instance;
        return instance;
    }

private:
    // Private constructor
    HubType()
        : ObjectType(LoadModel(HUB_TERRAIAN_MODEL), LoadTexture(SAND_TEXTURE)) {
        model_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture_;            // Set map diffuse texture

    }
};
class CoveType : public ObjectType {
public:
    CoveType(const CoveType& other) = delete;
    CoveType(CoveType&& other) = delete;
    CoveType& operator=(const CoveType& other) = delete;
    CoveType& operator=(CoveType&& other) = delete;

    // Singleton getter
    static CoveType& get_instance() {
        static CoveType instance;
        return instance;
    }

private:
    // Private constructor
    CoveType()
        : ObjectType(LoadModel(COVE_TERRAIN_MODEL), LoadTexture(SAND_TEXTURE)) {
        model_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture_;            // Set map diffuse texture

    }
};


class LagoonType : public ObjectType {
public:
    LagoonType(const LagoonType& other) = delete;
    LagoonType(LagoonType&& other) = delete;
    LagoonType& operator=(const LagoonType& other) = delete;
    LagoonType& operator=(LagoonType&& other) = delete;

    // Singleton getter
    static LagoonType& get_instance() {
        static LagoonType instance;
        return instance;
    }

private:
    // Private constructor
    LagoonType()
        : ObjectType(LoadModel(LAGOON_TERRAIN_MODEL), LoadTexture(SAND_TEXTURE)) {
        model_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture_;            // Set map diffuse texture

    }
};

class BayType : public ObjectType {
public:
    BayType(const BayType& other) = delete;
    BayType(BayType&& other) = delete;
    BayType& operator=(const BayType& other) = delete;
    BayType& operator=(BayType&& other) = delete;

    // Singleton getter
    static BayType& get_instance() {
        static BayType instance;
        return instance;
    }

private:
    // Private constructor
    BayType()
        : ObjectType(LoadModel(BAY_TERRAIN_MODEL), LoadTexture(SAND_TEXTURE)) {
        model_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture_;            // Set map diffuse texture

    }
};

class IsleType : public ObjectType {
public:
    IsleType(const IsleType& other) = delete;
    IsleType(IsleType&& other) = delete;
    IsleType& operator=(const IsleType& other) = delete;
    IsleType& operator=(IsleType&& other) = delete;

    // Singleton getter
    static IsleType& get_instance() {
        static IsleType instance;
        return instance;
    }

private:
    // Private constructor
    IsleType()
        : ObjectType(LoadModel(ISLE_TERRAIN_MODEL), LoadTexture(SAND_TEXTURE)) {
        model_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture_;            // Set map diffuse texture

    }
};

class ReefType : public ObjectType {
public:
    ReefType(const ReefType& other) = delete;
    ReefType(ReefType&& other) = delete;
    ReefType& operator=(const ReefType& other) = delete;
    ReefType& operator=(ReefType&& other) = delete;

    // Singleton getter
    static ReefType& get_instance() {
        static ReefType instance;
        return instance;
    }

private:
    // Private constructor
    ReefType()
        : ObjectType(LoadModel(REEF_TERRAIN_MODEL), LoadTexture(SAND_TEXTURE)) {
        model_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture_;            // Set map diffuse texture

    }
};

class TestType : public ObjectType {
public:
    TestType(const TestType& other) = delete;
    TestType(TestType&& other) = delete;
    TestType& operator=(const TestType& other) = delete;
    TestType& operator=(TestType&& other) = delete;

    // Singleton getter
    static TestType& get_instance() {
        static TestType instance;
        return instance;
    }

private:
    // Private constructor
    TestType()
        : ObjectType(LoadModelFromMesh(GenMeshCube(5, 5, 5)), LoadTexture(SAND_TEXTURE)) {
    }
};

#endif