#include "assignment7/Assignment7.h"
#include "common/core.h"

std::shared_ptr<Camera> Assignment7::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(resolution.x / resolution.y, 26.6f);
    camera->SetPosition(glm::vec3(0.f, -4.1469f, 0.73693f));
    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    return camera;
}


// Assignment 7 Part 1 TODO: Change the '1' here.
// 0 -- Naive.
// 1 -- BVH.
// 2 -- Grid.
#define ACCELERATION_TYPE 1



void addObject(std::shared_ptr<Scene> newScene, float reflectivity, std::string loc, glm::vec3 position, glm::vec3 orientation, float scale)
{
	std::shared_ptr<BlinnPhongMaterial> cubeMaterial = std::make_shared<BlinnPhongMaterial>();
	cubeMaterial->SetDiffuse(glm::vec3(1.f, 1.f, 1.f));
	cubeMaterial->SetSpecular(glm::vec3(0.6f, 0.6f, 0.6f), 40.f);
	cubeMaterial->SetReflectivity(reflectivity);

	std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
	std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh(loc, &loadedMaterials);

	for (size_t i = 0; i < cubeObjects.size(); ++i) {
		std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
		materialCopy->LoadMaterialFromAssimp(loadedMaterials[i]);
		cubeObjects[i]->SetMaterial(materialCopy);

		std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
		cubeSceneObject->AddMeshObject(cubeObjects[i]);
		//cubeSceneObject->Rotate(rotation, amtRotate);
		cubeSceneObject->SetBlenderCoordinates(position, orientation);
		//cubeSceneObject->SetPosition(position);
		cubeSceneObject->MultScale(scale);

		cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
		cubeSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});

		cubeSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});

		newScene->AddSceneObject(cubeSceneObject);
	}

}



std::shared_ptr<Scene> Assignment7::CreateScene() const
{
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

    // Material
    std::shared_ptr<BlinnPhongMaterial> cubeMaterial = std::make_shared<BlinnPhongMaterial>();
    cubeMaterial->SetDiffuse(glm::vec3(1.f, 1.f, 1.f));
    cubeMaterial->SetSpecular(glm::vec3(0.6f, 0.6f, 0.6f), 40.f);
    cubeMaterial->SetReflectivity(0.1f);

	//addObject(newScene, 0.1f, "background/background_better.obj", glm::vec3(0.f, 50.f, 0.f), glm::vec3(1.f, 0.f, 0.f),4.f);

	//addObject(newScene, 0.1f, "spiderman/untitled.obj", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f),  0.05f);

	addObject(newScene, 0.1f, "TEST/building_test.obj", glm::vec3(0.f, 10.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f);

	
	/*

	//THEIR OBJECTS
	std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;

    //std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("CornellBox/CornellBox-Assignment7-Alt.obj", &loadedMaterials);
	//std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("BB8 New/bb8.obj", &loadedMaterials);
	//std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("modern_office_building/OfficeTowerV1.obj", &loadedMaterials);
	//std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("2wtc_obj/2wtc.obj", &loadedMaterials);
	//std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("Skyscraper.A/SkyA.obj", &loadedMaterials);
	std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh("background/background_better.obj", &loadedMaterials);
    for (size_t i = 0; i < cubeObjects.size(); ++i) {
        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
        materialCopy->LoadMaterialFromAssimp(loadedMaterials[i]);
		cubeObjects[i]->SetMaterial(materialCopy);

        std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
        cubeSceneObject->AddMeshObject(cubeObjects[i]);
        cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI );
		//cubeSceneObject->Rotate(glm::vec3(0.f, 1.f, 0.f), -4.0f);
		cubeSceneObject->SetPosition(glm::vec3(0.f, 50.f, 0.f));
		cubeSceneObject->MultScale(4.f);

        cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
        cubeSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });

        cubeSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });


        //newScene->AddSceneObject(cubeSceneObject);
    }

	//glm::vec3[] rotates = 

	addObject(newScene, 0.1f, "sky/sky.obj", glm::vec3(0.f, 50.f, 0.f), glm::vec3(1.f, 0.f, 0.f), PI, 4.f);

	std::vector<std::shared_ptr<aiMaterial>> loadedMaterials3;
	cubeMaterial->SetReflectivity(0.0f);

	std::vector<std::shared_ptr<MeshObject>> background2 = MeshLoader::LoadMesh("sky/sky.obj", &loadedMaterials3);
	for (size_t i = 0; i < background2.size(); ++i) {
		std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
		materialCopy->LoadMaterialFromAssimp(loadedMaterials3[i]);
		background2[i]->SetMaterial(materialCopy);

		std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
		cubeSceneObject->AddMeshObject(background2[i]);
		cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI);
		cubeSceneObject->Rotate(glm::vec3(0.f, 0.f, 1.f), PI/2);
		//cubeSceneObject->Rotate(glm::vec3(0.f, 1.f, 0.f), -4.0f);
		cubeSceneObject->SetPosition(glm::vec3(-7.f, 5.f, 0.f));
		cubeSceneObject->MultScale(3.f);

		cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
		cubeSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});

		cubeSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});


		newScene->AddSceneObject(cubeSceneObject);
	}

	for (size_t i = 0; i < background2.size(); ++i) {
		std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
		materialCopy->LoadMaterialFromAssimp(loadedMaterials3[i]);
		background2[i]->SetMaterial(materialCopy);

		std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
		cubeSceneObject->AddMeshObject(background2[i]);
		cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI);
		cubeSceneObject->Rotate(glm::vec3(0.f, 0.f, 1.f), -PI / 2);
		//cubeSceneObject->Rotate(glm::vec3(0.f, 1.f, 0.f), -4.0f);
		cubeSceneObject->SetPosition(glm::vec3(7.f, 5.f, 0.f));
		cubeSceneObject->MultScale(3.f);

		cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
		cubeSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});

		cubeSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});


		newScene->AddSceneObject(cubeSceneObject);
	}

	cubeMaterial->SetReflectivity(0.0f);

	for (size_t i = 0; i < cubeObjects.size(); ++i) {
		std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
		materialCopy->LoadMaterialFromAssimp(loadedMaterials[i]);
		cubeObjects[i]->SetMaterial(materialCopy);

		std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
		cubeSceneObject->AddMeshObject(cubeObjects[i]);
		cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI);
		cubeSceneObject->Rotate(glm::vec3(0.f, 0.f, 1.f), PI);
		//cubeSceneObject->Rotate(glm::vec3(0.f, 1.f, 0.f), -4.0f);
		cubeSceneObject->SetPosition(glm::vec3(0.f, -5.f, 0.f));
		cubeSceneObject->MultScale(3.f);

		cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
		cubeSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});

		cubeSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});


		newScene->AddSceneObject(cubeSceneObject);
	}


	cubeMaterial->SetReflectivity(0.6f);
	//BUILDING
	std::vector<std::shared_ptr<aiMaterial>> loadedMaterials2;

	
	//std::vector<std::shared_ptr<MeshObject>> cubeObjects2 = MeshLoader::LoadMesh("robot/model.obj", &loadedMaterials2);
	std::vector<std::shared_ptr<MeshObject>> cubeObjects2 = MeshLoader::LoadMesh("skyscraper_01_obj/windows.obj", &loadedMaterials2);
	for (size_t i = 0; i < cubeObjects2.size(); ++i) {
		std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
		materialCopy->LoadMaterialFromAssimp(loadedMaterials2[i]);
		cubeObjects2[i]->SetMaterial(materialCopy);

		std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
		cubeSceneObject->AddMeshObject(cubeObjects2[i]);
		cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
		//cubeSceneObject->Rotate(glm::vec3(0.f, 0.f, 1.f), -PI /1.2f);
		//cubeSceneObject->Rotate(glm::vec3(0.f, 1.f, 0.f), -4.0f);
		cubeSceneObject->SetPosition(glm::vec3(1.f, 7.f, -2.f));
		cubeSceneObject->MultScale(0.1f);

		cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
		cubeSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});

		cubeSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});


		newScene->AddSceneObject(cubeSceneObject);
	}*/

    // Lights
	/*
	std::shared_ptr<AreaLight> aLight = std::make_shared<AreaLight>(glm::vec2(0.5f, 0.5f));
	aLight->SetSamplerAttributes(glm::vec3(2.f, 2.f, 1.f), 4);
	aLight->SetPosition(glm::vec3(0.01909f, 0.0101f, 1.97028f));
	aLight->SetLightColor(glm::vec3(1.f, 1.f, 1.f));
	newScene->AddLight(aLight);*/

    std::shared_ptr<Light> pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(glm::vec3(0.01909f, 0.0101f, 1.97028f));
    pointLight->SetLightColor(glm::vec3(1.f, 1.f, 1.f));

#if ACCELERATION_TYPE == 0
    newScene->GenerateAccelerationData(AccelerationTypes::NONE);
#elif ACCELERATION_TYPE == 1
    newScene->GenerateAccelerationData(AccelerationTypes::BVH);
#else
    UniformGridAcceleration* accelerator = dynamic_cast<UniformGridAcceleration*>(newScene->GenerateAccelerationData(AccelerationTypes::UNIFORM_GRID));
    assert(accelerator);
    // Assignment 7 Part 2 TODO: Change the glm::ivec3(10, 10, 10) here.
    accelerator->SetSuggestedGridSize(glm::ivec3(3, 3, 3));
#endif    
    newScene->AddLight(pointLight);

    return newScene;

}
std::shared_ptr<ColorSampler> Assignment7::CreateSampler() const
{
    std::shared_ptr<JitterColorSampler> jitter = std::make_shared<JitterColorSampler>();
    jitter->SetGridSize(glm::ivec3(1, 1, 1));
    return jitter;
}

std::shared_ptr<class Renderer> Assignment7::CreateRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<ColorSampler> sampler) const
{
    return std::make_shared<BackwardRenderer>(scene, sampler);
}

int Assignment7::GetSamplesPerPixel() const
{
    return 1;
}

bool Assignment7::NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex)
{
    return true;
}

int Assignment7::GetMaxReflectionBounces() const
{
    return 2;
}

int Assignment7::GetMaxRefractionBounces() const
{
    return 4;
}

glm::vec2 Assignment7::GetImageOutputResolution() const
{
    return glm::vec2(640.f, 480.f);
}
