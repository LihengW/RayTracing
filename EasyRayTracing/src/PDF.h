#pragma once
#include "utility.h"
#include "Objects.h"

class PDF
{
    // the responsiblity of pdf includes:
    // 1. returning a random direction weighted by the internal PDF distribution,
    // 2. returning the corresponding PDF distribution value in that direction.
public:
	PDF() {}
	virtual ~PDF() {}

	virtual float value(const glm::vec3& direction) const = 0;
	virtual glm::vec3 generate() const = 0;
};

class SpherePDF : public PDF {
    // sphere uniform distribution
public:
    SpherePDF() { };

    float value(const glm::vec3& direction) const override {
        return 1 / (4 * Utility::pi);
    }

    glm::vec3 generate() const override {
        return Utility::RandomUnitVector();
    }
};

class CosinePDF : public PDF {
public:
    // cosine distribution (the possiblity is related to cos(theta))
    CosinePDF(const glm::vec3& w) { uvw.build_from_w(w); }

    float value(const glm::vec3& direction) const override {
        auto cosine_theta = glm::dot(glm::normalize(direction), uvw.w());
        return fmaxf(0.0f, cosine_theta / Utility::pi);
    }

    glm::vec3 generate() const override {
        return uvw.local(Utility::RandomCosDir());
    }

private:
    ONB uvw;
};

class ObjectPDF : public PDF {
public:
    ObjectPDF(const std::shared_ptr<Object>& _object_ptr, const glm::vec3& _origin)
        : origin(_origin), object_ptr(_object_ptr)
    {}

    float value(const glm::vec3& direction) const override {
        // need to be fixed
        return std::dynamic_pointer_cast<Quad>(object_ptr)->pdf_value(origin, direction);
    }

    glm::vec3 generate() const override {
        // need to be fixed
        return std::dynamic_pointer_cast<Quad>(object_ptr)->random(origin);
    }
    

private:
    const std::shared_ptr<Object>& object_ptr;
    glm::vec3 origin;
};

class MixturePDF : public PDF {
public:
    MixturePDF(std::shared_ptr<PDF> pdf_ptr1, std::shared_ptr<PDF> pdf_ptr2)
    {
        pdfs[0] = pdf_ptr1;
        pdfs[1] = pdf_ptr2;
    }

    float value(const glm::vec3& direction) const override {
        return 0.5f * pdfs[0]->value(direction) + 0.5f * pdfs[1]->value(direction);
    }

    glm::vec3 generate() const override {
        if (Utility::RandomFloat() > 0.5f)
        {
            return pdfs[0]->generate();
        }
        else
        {
            return pdfs[1]->generate();
        }
    }


private:
    std::shared_ptr<PDF> pdfs[2];
};