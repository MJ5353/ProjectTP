# ProjectTP

UE 5.7 타격감·카메라 연출 데모 (포트폴리오용). Third Person C++ 템플릿 기반.
"입력 반응 → 타격 → 피격 반응 → 화면 연출" 흐름 전체를 적 1종과의 짧은 전투로 보여준다.

## 작업 방식
- 빌드는 Claude가 직접 돌리지 않는다. 빌드·에디터 확인이 필요하면 무엇을 왜 확인해야 하는지 알리고 사용자가 진행한다.
- 코드는 사용자 PC의 로컬 파일만 고친다. 커밋·푸시는 사용자가 하거나 지시할 때만 한다. 클라우드에서 실행 중이면 먼저 PC 폴더 연결을 요청한다.
- PR 리뷰는 엔진이 없는 클라우드에서 돌기 때문에 컴파일 검증이 아니라 코드 읽기 기준이다.
- 기본 브랜치는 `main`. 기능 하나당 브랜치 하나(`feat/<기능>`, 예: `feat/hitstop`)와 PR 하나. PR마다 Claude 리뷰를 받고, 리뷰를 수용/거절한 이유를 PR에 남긴다.
- 커밋 메시지는 한국어로, 무엇을 왜 바꿨는지 쓴다.

## 폴더 규칙
| 경로 | 내용 | git |
|---|---|---|
| `Source/ProjectTP/` | 내 C++ 코드. 기능별 하위 폴더로 나눈다 (예: `Common/`, `AbilitySystem/`, `HitFeel/`, `Camera/`) | 포함 |
| `Source/ProjectTP/Variant_Combat/` | Epic 템플릿 코드. 참고만 하고 수정하지 않는다 | 포함 |
| `Content/ProjectTP/` | 직접 만든 몽타주·BP·머티리얼·DataTable·맵 | 포함 (LFS) |
| `Content/ThirdParty/` | Paragon 애니, Infinity Blade 이펙트 등 외부 에셋 | 제외 |
| `Data/` | 타격감 수치 CSV (`HitFeel.csv` 등). DataTable 원본 | 포함 |

- 외부 에셋은 `Content/ThirdParty/` 밖에 두지 않는다. 내 에셋이 외부 에셋을 참조하는 것은 괜찮다.
- 외부 에셋을 복제·리타겟·Bake해서 만든 에셋(리타겟 애니, Duplicate한 머티리얼·Niagara 등)도 `Content/ThirdParty/`에 저장한다. `Content/ProjectTP/`에는 외부 에셋을 참조만 하는 에셋(몽타주, 직접 만든 머티리얼 등)을 둔다.
- 내 코드의 include는 `"ProjectTP/Common/TPGameplayTags.h"`처럼 모듈 기준 전체 경로로 쓴다. cpp가 자기 헤더를 include할 때만 이름만 쓴다. 새 폴더를 `PublicIncludePaths`에 추가하지 않는다 (남아 있는 항목은 Epic 템플릿용).

## 코드 규칙 (UE C++)
- 코드 스타일은 `Memo/CodeStyle.md`(ProjectSP 스타일)를 따른다. 새로 쓰는 코드와 고치는 코드 모두 적용한다.
- 핵심 로직은 C++. BP는 에셋 연결용 자식 클래스만 둔다 (AnimBP, GameplayCue 노티파이, 에셋 레퍼런스 지정). BP는 바이너리라 PR 리뷰가 안 된다.
- Epic 코딩 표준을 따른다: 타입 접두사(A/U/F/E/I/T), bool은 `b` 접두사, PascalCase.
- UObject 멤버 포인터는 `UPROPERTY()` + `TObjectPtr<>`. GC에 잡히지 않는 raw 포인터를 두지 않는다.
- 로그는 `LogMj` 카테고리(`Common/TPLog.h`)를 쓴다. `LogProjectTP`는 Epic 템플릿 코드용으로 남긴다.
- 수치를 코드에 박지 않는다. 타격감 수치는 `Data/*.csv` DataTable, 에셋 참조는 DataAsset.
- Tick보다 타이머·이벤트·AnimNotify를 우선한다. 프레임에 따라 결과가 달라지는 로직을 만들지 않는다.

## GAS 구조 원칙
- 기본 뼈대(ASC, 어빌리티 베이스)는 ProjectSP에서 이식한다.
- 공격·회피 같은 행동은 GameplayAbility로 만들고, 몽타주는 `PlayMontageAndWait` 태스크로 재생한다.
- 데미지는 GameplayEffect + SetByCaller로 전달한다. 어트리뷰트(HP)를 코드에서 직접 바꾸지 않는다.
- 연출(카메라 셰이크·이펙트·사운드·히트 플래시)은 GameplayCue로 묶는다. 어빌리티에서 이펙트·사운드를 직접 스폰하지 않는다.
- 입력 버퍼·캔슬 구간은 AnimNotifyState로 표시하고, 판단은 어빌리티가 한다.
- GameplayTag는 C++ 네이티브 태그(`UE_DECLARE_GAMEPLAY_TAG_EXTERN` / `UE_DEFINE_GAMEPLAY_TAG`)로 `Common/TPGameplayTags`에 모아 정의한다.

## 타격감 구현 주의점
- 히트스톱 해제 타이머는 실제 시간 기준으로 잰다. 시간 배속(Time Dilation)을 걸면 GAS 타이머와 몽타주도 같이 느려진다.
- `Data/HitFeel.csv`는 공격 하나당 1행이다. PR에서 튜닝 이력이 보이도록 수치 변경은 CSV에서만 한다.
- CSV에 열을 추가하면 대응하는 `FTableRowBase` 구조체도 같은 커밋에서 바꾼다.

## 커밋 전 훅
처음 한 번 저장소 루트에서 Git Bash로 설치한다. 훅 파일을 바꾸면 다시 실행한다.
```
sh Tools/GitHooks/install-hooks.sh
```
검사 항목: `Content/ThirdParty`·`Content/Developers` 커밋 차단, LFS 대상이 포인터로 올라가는지 확인, LFS가 아닌 1MB 넘는 바이너리 차단, `Data/*.csv` 열 개수·행 이름 중복 검사.
