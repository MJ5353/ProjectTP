# ProjectTP

UE5 C++ 타격감·카메라 연출 데모. 입력 반응부터 타격, 피격 반응, 화면 연출까지 전투의 "손맛"을 만드는 흐름 전체를 적 1종과의 짧은 전투로 보여줍니다.

<!-- TODO: 하이라이트 영상 (15~30초) -->
> 하이라이트 영상 준비 중

- 엔진: Unreal Engine 5.7 (C++)
- 기간: 2주 (진행 중)

## 시연 구성
약공 3타 → 회피 캔슬 → 강공격(넉백) → 마무리 일격

| 동작 | 연출 |
|---|---|
| 약공 3타 | 짧은 히트스톱, 타수마다 커지는 카메라 셰이크 |
| 회피 | 입력 버퍼·캔슬 구간으로 콤보 도중 끊고 빠지기 |
| 강공격 | 긴 히트스톱, 넉백, FOV 킥·방사형 블러 |
| 마무리 일격 | 연출 카메라 + 슬로모션 |
| 적 피격 | 방향별 피격 리액션, 넉백·넉다운, 히트 플래시 |

<!-- TODO: 연출 켬/끔 비교 클립 -->

## 구조
- **GAS 기반 전투**: 공격·회피는 GameplayAbility, 데미지는 GameplayEffect(SetByCaller), 연출(셰이크·이펙트·사운드·히트 플래시)은 GameplayCue로 분리
- **데이터로 튜닝**: 히트스톱 길이, 셰이크 세기 같은 타격감 수치는 `Data/HitFeel.csv` → DataTable. 공격 하나당 1행이라 PR에서 튜닝 이력이 그대로 보입니다
- **C++ 중심**: 핵심 로직은 C++, 블루프린트는 에셋 연결용 자식 클래스만 사용
- GAS 기본 뼈대(ASC, 어빌리티 베이스)는 이전 프로젝트 [ProjectSP](https://github.com/MJ5353/ProjectSP_Public)의 구조를 재사용했습니다

<!-- TODO: 구조 다이어그램, 주요 클래스 링크 -->

## AI 활용 개발
기능 하나당 PR 하나를 만들고, PR마다 Claude 코드 리뷰를 받았습니다.
- 규칙: [`CLAUDE.md`](CLAUDE.md) (UE C++ 규칙, GAS 구조 원칙)
- 커밋 전 검사: [`Tools/GitHooks/pre-commit`](Tools/GitHooks/pre-commit) (외부 에셋 커밋 차단, LFS 확인, CSV 형식 검사)
- PR 리뷰: [`.github/workflows/claude-review.yml`](.github/workflows/claude-review.yml)

<!-- TODO: 리뷰를 수용/거절한 판단 사례 2~3개 (PR 링크) -->

## 에셋 출처
외부 에셋은 저장소에 포함하지 않습니다 (`Content/ThirdParty/`는 git 제외).

| 에셋 | 출처 | 저장소 포함 |
|---|---|---|
| 프로젝트 템플릿, UE5 마네킹(Manny·Quinn) | UE 5.7 Third Person 템플릿 (Epic Games). `Variant_Combat` 코드는 참고용 원본 유지 | 포함 |
| 애니메이션 | Paragon (Epic Games), UE5 마네킹 리타겟 배포본 | 미포함 |
| 타격 이펙트 | Infinity Blade (Epic Games) | 미포함 |
| 대검 메시 | 미정 | 미포함 |
| 사운드 | 미정 | 미포함 |

## 실행
1. UE 5.7과 UE 5.7이 지원하는 Visual Studio(C++ 게임 개발 워크로드)를 설치합니다.
2. 저장소를 받고 Git LFS로 에셋을 받습니다: `git lfs pull`
3. `ProjectTP.uproject` 우클릭 → Generate Visual Studio project files → 빌드 후 실행합니다.

외부 에셋이 없으면 애니메이션·이펙트 참조가 끊긴 채로 열립니다. 위 출처의 에셋을 `Content/ThirdParty/` 아래 같은 경로로 넣으면 참조가 다시 연결됩니다.
