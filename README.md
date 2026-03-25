


# Ray Tracing Renderer (c++)

C++로 구현한 CPU기반 RayTracing 입니다.
Ray-Sphere Intersection, 다양한 Material(Diffuse/Metal/Dielectric),확률 근사 기반의 재귀적 RayTracing을 통해
물리 기반 렌더링(PBR)을 구현하였습니다.

---
## Result
<img width="400" height="225" alt="image" src="https://github.com/user-attachments/assets/c287213c-a4e4-46f9-a847-d76076c214ea" />
<img width="400" height="225" alt="metal" src="https://github.com/user-attachments/assets/6c8315ff-4250-43ac-8baa-bef1be8e4fd3" />


## Features
- Ray-Sphere Intersection 구현
- Diffuse (Lambertian) 재질
- Metal (Reflection + Fuzz) 재질
- Dielectric (Refraction + Schlick approximation)
- Recursive Ray Tracing (Depth 제한)
- Anti-Aliasing (Multi Sampling)
- Gamma Correction 적용
- Depth of Field (Defocus Blur)

---
## Key Concepts

### Ray Equation
Ray는 다음과 같이 표현됩니다.

P(t) = Origin + t * Direction

### Diffuse Reflection (Lambertian)
표면 법선과 랜덤 단위 벡터를 더하여 난반사를 구현했습니다.

- RandomUnitVector() 사용
- 자연스러운 빛 확산 표현

### Metal Reflection
반사 벡터를 기반으로 금속 재질을 구현했습니다.

- Reflect(v, n) 사용
- fuzz factor를 통해 표면 거칠기 표현

### Dielectric (Refraction)
유리 재질의 굴절 및 반사를 구현했습니다.

- Snell’s Law 기반 굴절
- Schlick approximation을 이용한 반사율 계산
- 내부 전반사 처리

---
##  Architecture

렌더러는 다음과 같은 구조로 설계되었습니다.

- Vec3  
  → 벡터 연산 및 수학 연산 처리

- Ray  
  → 광선 정의 (Origin + Direction)

- Hittable (Interface)  
  → 충돌 가능한 객체 추상화

- Sphere  
  → 구 형태의 충돌 객체

- HittableList  
  → 여러 객체를 관리하는 컨테이너

- Material (Interface)  
  → 재질 추상화

    - Lambertian (Diffuse)
    - Metal (Reflection)
    - Dielectric (Refraction)

- Camera  
  → Ray 생성 및 Viewport 구성, Sampling 처리

-- 인터페이스 기반 구조를 통해 객체 및 재질 확장이 용이하도록 설계했습니다.

---

##  Rendering Process

1. Camera에서 Ray 생성
2. Ray가 Scene(HittableList)과 충돌 검사
3. 충돌 시 Material에 따라 Scatter 처리
4. 재귀적으로 Ray 추적 (depth 제한)
5. Sampling을 통한 색상 누적
6. Gamma Correction 후 출력

---
##  Improvements

- Recursive depth 제한을 통한 성능 제어
- Sampling 수 조절을 통한 품질/성능 trade-off 고려
- Defocus Blur를 통한 Depth of Field 구현

---
## What I Learned
- 단순한 렌더링이 아닌 물리 기반 빛의 상호작용 이해
- 객체지향 설계를 통해 확장 가능한 렌더러 구조 설계
- 확률 기반 렌더링 기법

---
##  Future Work
- CUDA를 사용한 GPU 멀티스레딩 적용

---


