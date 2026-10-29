# ProjectSP 코드 스타일

최종 수정: 26.10.29

ProjectSP에서 쓰던 C++ 스타일을 정리한 것. ProjectTP도 이 스타일을 따른다. 요지는 **주석은 줄이고, 구분선과 짧은 라벨로 구조를 보여 주는 코드**.

## 파일

- 인코딩: UTF-8 **BOM 있음** (ProjectSP는 BOM 없음, ProjectTP는 BOM 유지로 결정).
- 줄바꿈은 git `.gitattributes`(`* text=auto`)에 맡긴다.
- 클래스 접두사: 프로젝트 약어 (`Sp`, `TP`). 클래스에 `<모듈>_API`를 붙인다 (`PROJECTSP_API`, `PROJECTTP_API`).

## include

- cpp가 자기 헤더를 include할 때만 이름만 쓴다 (`#include "TPLog.h"`).
- 그 외 프로젝트 헤더는 모듈 기준 전체 경로 (`#include "ProjectTP/Common/TPGameplayTags.h"`).
- Build.cs에 새 폴더를 `PublicIncludePaths`로 추가하지 않는다.
- Build.cs 모듈 목록은 `// Input`, `// GAS`, `// AI`, `// UMG` 같은 주석으로 묶는다.

## 구분선과 라벨

- 큰 구분선 `// ==================================================`
  - 헤더: include·전방 선언 바로 아래 (델리게이트·enum·클래스 앞), 그리고 enum·델리게이트 묶음과 클래스 사이.
  - cpp: include 바로 아래 (첫 함수·namespace 앞).
- 작은 구분선 `// ------------------------------------------------`
  - 클래스 안에서 데이터와 함수 사이, 델리게이트 묶음과 클래스 사이.
- 섹션 라벨
  - 큰 묶음: `// Core ----`, `// Dead ----` (대문자로 시작).
  - 작은 묶음: `// tag event`, `// unit manage` (소문자, 구분선 없음).
  - cpp에도 헤더와 같은 라벨을 달아 순서를 맞춘다.

## 헤더 순서

1. 데이터: component → setting → public → replicate → runtime → cpp_uobject → UPROPERTY 없는 멤버
2. 공개 델리게이트
3. 생성자·엔진 override
4. 기능 묶음별 함수. 묶음마다 `public:` / `protected:`를 다시 쓴다.
5. Get 묶음 (한 줄 인라인 getter `{ return X; }`)

접근 지정자 다음 줄에 빈 줄을 두지 않는다.

## 주석

- 클래스·멤버에 `/** */` 설명을 달지 않는다. `/** */`는 매크로 같은 재사용 정의에만.
- 본문 주석은 한국어 한 줄, 코드만 봐서는 모를 "왜"만 쓴다.
  - 예: `// 다음 대미지가 누적되지 않도록 즉시 초기화`

## 문법

- 한 문장 if·for·else는 중괄호 없이 다음 줄 들여쓰기. 여러 줄이면 중괄호.
  ```cpp
  if (!ASC || !ASC->IsOwnerActorAuthoritative())
  	return;
  ```
- 실수 리터럴은 `0.f`.
- 출력 인자는 포인터 + `Out` 접두사 (`FTPGrantedAbilityHandles* OutGrantedHandles`), 함수 안에서 null 검사.
- 값 인자의 `const`는 cpp 정의에만 붙인다 (헤더 `float DeltaTime`, cpp `const float DeltaTime`).
- 파일 안에서만 쓰는 헬퍼는 이름 없는 `namespace { }`. 매크로는 `do { } while (false)`.
- 디버그 전용 코드는 `#if !UE_BUILD_SHIPPING`.

## 이름

- 약어는 첫 글자만 대문자: `Hp` (`CurrentHp`, `RestoreHp`).
- 네트워크 방향은 함수 이름 끝에 `_Server` / `_Client` (`SetUnitActive_Server`).
  - 서버(또는 클라)에서만 불러야 하는 진입 함수에 붙인다. 안에서 권한을 검사해 조용히 끝나더라도 붙인다 (`SetUp_Server`, `RestoreHp_Server`). 그 쪽에서만 도는 처리 안에서 부르는 내부 함수(`NotifyDamageApplied`)와 protected 함수는 붙이지 않는다.
- 어빌리티 찾기 함수는 `...ByTag` (`FindAbilitySpecByTag`, `TryActivateAbilityByTag`).
- GameplayTag
  - 네이티브 태그를 네임스페이스 하나에 모은다 (`TPGameplayTags`, ProjectTP는 `Common/TPGameplayTags`).
  - 변수 이름은 `분류Tag_이름` (`StateTag_Dead`, `EventTag_Hit`, `AbilityKeyTag`).
  - 소문자 주석으로 묶는다 (`// ability key`, `// state`).
- 로그
  - 공용 카테고리 `LogMj` (ProjectTP는 `Common/TPLog.h`).
  - 파일 안에서만 쓰는 로그는 `DEFINE_LOG_CATEGORY_STATIC`.
  - ProjectTP의 `LogProjectTP`는 Epic 템플릿 코드용으로만 남긴다.

## 생성자

```cpp
// 헤더
ATPUnit(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

// cpp
ATPUnit::ATPUnit(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}
```

## UPROPERTY

공통: `Category = "..."`, `meta = (...)`, `ReplicatedUsing = ...`는 `=` 양쪽을 띄운다. UObject 멤버 포인터는 `TObjectPtr<>`, 클래스 참조는 `TSubclassOf<>`.

| 용도 | 쓰는 곳 | 지정자 |
|---|---|---|
| component | 생성자에서 `CreateDefaultSubobject`로 만드는 컴포넌트. `protected`에 둔다 | `UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MJ - Component")` |
| setting | 클래스 기본값(BP 자식)에서만 정하는 값. DataAsset 필드, 에셋 참조 | `UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting")` |
| setting_tag | setting 중 GameplayTag 필드. 선택지를 좁힌다 | `UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MJ - Setting", meta = (Categories = "Ability.Key"))` |
| public | 기본값과 레벨 인스턴스 양쪽에서 조정. BP는 읽기만 | `UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MJ - Public")` |
| runtime | 실행 중 C++가 계산·갱신, PIE 디테일에서 보고 싶은 값. BP는 읽기만 | `UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "MJ - Runtime")` |
| cpp_uobject | 에디터에 안 보여도 되는 C++ 내부 UObject 참조·캐시 (내가 소유). UObject를 담는 USTRUCT 멤버도 | `UPROPERTY(Transient)` |
| replicated | 서버 값을 클라에 그대로 복제 | `UPROPERTY(BlueprintReadOnly, Replicated, Category = "MJ - Replicate")` |
| repnotify | 복제 뒤 클라 반응이 필요한 값 | `UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_X, Category = "MJ - Replicate")` |
| 메타 어트리뷰트 | 복제하지 않고 계산에만 쓰는 GAS 어트리뷰트 (Damage) | `UPROPERTY(BlueprintReadOnly, Category = "MJ - Runtime")` |

- `private`에 둔 component만 `meta = (AllowPrivateAccess = "true")`를 붙인다.
- replicated·repnotify는 `GetLifetimeReplicatedProps`에 `DOREPLIFETIME`을 같이 추가한다.
- repnotify는 `UFUNCTION() void OnRep_X(const T& OldX);`를 같이 선언한다.
- GAS 어트리뷰트는 `DOREPLIFETIME_CONDITION_NOTIFY(..., COND_None, REPNOTIFY_Always)`와 `GAMEPLAYATTRIBUTE_REPNOTIFY`를 쓴다.

UPROPERTY 없이 그냥 멤버로 두는 것:
- 내가 소유하지 않는 액터·컴포넌트 참조: `TWeakObjectPtr<>`
- `FTimerHandle`, `FDelegateHandle`, bool 플래그 (`bool bX = false;`처럼 헤더에서 초기화)
- 숫자·태그 같은 UObject가 아닌 상태
- 네이티브 델리게이트 (`DECLARE_MULTICAST_DELEGATE...`)

Category `"MJ - "` 접두사는 방향만 정했고 세부는 보류 중 (26.10.29).

## UFUNCTION

- BP에 노출하는 UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintImplementableEvent, BlueprintNativeEvent)에는 Category를 항상 붙인다.
  - 예: `UFUNCTION(BlueprintCallable, Category = "MJ - Ability")`
- Category 이름은 기능 묶음 단위로 쓴다 (`"MJ - Ability"`, `"MJ - Unit"` 등).
- OnRep처럼 BP에 노출하지 않는 `UFUNCTION()`에는 붙이지 않는다.

## Rider 라이브 템플릿

| 단축어 | 내용 |
|---|---|
| `uobjconstructor` | `const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()` |
| `uproperty_component` | component 지정자 |
| `uproperty_setting` | setting 지정자 |
| `uproperty_setting_tag` | setting_tag 지정자, 태그 자리는 `$TAG$` (26.10.29 신규) |
| `uproperty_public` | public 지정자 (26.10.29 BlueprintReadWrite → ReadOnly) |
| `uproperty_runtime` | runtime 지정자 (26.10.29 BlueprintReadWrite → ReadOnly) |
| `uproperty_cpp_uobject` | `UPROPERTY(Transient)` |
| `uproperty_replicated` | replicated 지정자 |
| `uproperty_repnotify` | repnotify 지정자, 이름 자리는 `$NAME$` |

## 예시

```cpp
#include "TPGrantedHandles.h"
#include "ProjectTP/Ability/Core/TPAbilitySystemComponent.h"

// ==================================================

void FTPGrantedAbilityHandles::TakeFromAbilitySystem(UTPAbilitySystemComponent* ASC)
{
	if (!ASC || !ASC->IsOwnerActorAuthoritative())
		return;

	for (const TPair<FGameplayTag, FGameplayAbilitySpecHandle>& Pair : TagToAbilitySpecHandle)
	{
		if (Pair.Value.IsValid())
			ASC->ClearAbility(Pair.Value);
	}

	TagToAbilitySpecHandle.Reset();
}
```
