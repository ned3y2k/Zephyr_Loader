# 서풍의 광시곡 로더
## 소개
1998년도 [Softmax](https://namu.wiki/w/%EC%86%8C%ED%94%84%ED%8A%B8%EB%A7%A5%EC%8A%A4)에서 발매된 게임인 창세기전 외전 서풍의 광시곡 로더입니다.  
온라인 배포를 목적으로 하는 것이 아니며, 개인 연구 및 연구 결과 공유를 위한 목적이므로 원본 디스크 파일에 대한 질문은 받지않겠습니다.  
이 프로그램을 사용 및 원본 이미지 파일과 같이 배포하는 등 기타 행위로 인하여 발생하는 법적 책임은 당사자 및 배포자에게 있음을 알립니다.

## 빌드 환경
- Windows 10 21H1(19043.1165)
- Visual Studio 2019 Community(16.11.2)

## 설치 되지 않는 서풍의 광시곡 ㅠㅠ
Install Shield 구버전으로 설치프로그램이 구성되어 있어 Windows 10 64Bit에서는 설치가 진행되지 않습니다.
다행이지만 케케묵은 기억 속에 이 게임은 레지스트리나 별도의 시스템 파일(코덱 제외)이 없어도 실행이 된다는 사실은 알고 있었습니다.  
Install Shield Cabinet File의 추출을 위해 검색해본 결과 [Universal Extractor](https://www.legroom.net/software/uniextract)의 추천이 있어서 그대로 진행하였습니다. 

## 지원되지 않는 코덱으로 인한 실행 불가 ㅠㅠ
Windows에서 보안문제로 Indeo 코덱을 기본으로 제공하지 않으며 잘 사용되지 않아 더 이상 찾아보기도 힘듭니다.
일단 설치하여야 분석 가능하지만 Windows 내장 코덱 파일로 동영상들을 재인코딩하여도 무방합니다.  
- [컴퓨터에 설치되어 있는 코덱을 확인하는 방법](https://support.microsoft.com/en-us/windows/codecs-faq-392483a0-b9ac-27c7-0f61-5a7f18d408af)
- [Indeo 코덱 보안 이슈 공지](https://support.microsoft.com/ko-kr/topic/microsoft-%EB%B3%B4%EC%95%88-%EA%B6%8C%EA%B3%A0-indeo-%EC%BD%94%EB%8D%B1%EC%9D%98-%EC%B7%A8%EC%95%BD%EC%84%B1%EC%9C%BC%EB%A1%9C-%EC%9D%B8%ED%95%9C-%EC%9B%90%EA%B2%A9-%EC%BD%94%EB%93%9C-%EC%8B%A4%ED%96%89-2009%EB%85%84-12%EC%9B%94-8%EC%9D%BC-680127c6-f4fd-e533-b641-6ab3a25fb4f4)
``` 샤나인코더 기준 설정 테스트 완료 값(인코딩 설정)
 -f avi
 -c:v mpeg4 -q:v 2.5
 -c:a libmp3lame -ac 2 -b:a 128k
 -sn -map_metadata -1 -map_chapters -1
 ```
 ``` 동영상 파일 목록
설치경로\objs\*.obi
disk?\maps
OPEN.zmk
LOGO.zmk
01ASCA.zmk
02ASURA.zmk
03FLAG.zmk
04CYRUP.zmk
05DAKA.zmk
06END1.zmk
07END2.zmk
08INFRN.zmk
09MAP1.zmk
10MAP2.zmk
11VS1.zmk
12VS2.zmk
 ```

## 디스크 구성
서풍의 광시곡은 3장의 Compact Disc([Mixed Mode CD](https://en.wikipedia.org/wiki/Mixed_Mode_CD))로 구성되어 있으며 멀티 볼륨 게임 특성상 중복 오디오 및 데이터가 존재합니다.

## 일반적인 고전 게임 디스크 판단 루틴
1. <span id="GetLogicalDrives">초기화시 시스템에 설치된 볼륨 확인</span>
    - [GetLogicalDrives](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getlogicaldrives)
2. <span id="GetDriveTypeA">시스템에 설치된 볼륨 드라이브 유형 확인(DRIVE_CDROM=5)</span>
    - [GetDriveTypeA](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdrivetypea)
3. <span id="GetVolumeInformationA">디스크 레이블 확인</span>
    - [GetVolumeInformationA](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getvolumeinformationa)
4. 파일 존재 확인
    - [PathFileExistsA](https://docs.microsoft.com/en-us/windows/win32/api/shlwapi/nf-shlwapi-pathfileexistsa)
    - [CreateFileA](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea)
    - [FindFirstFileA](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-findfirstfilea)/[FindNextFileA](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-findnextfilea)

## 서풍의 광시곡 디스크 판단 루틴 분석(OllyDbg, Rohitab API Monitor v2 이용)
0. 다른 디스크기 필요시 disk%d.Bmp 파일을 로딩하여 필요한 디스크를 표시한다.
    - [CreateFileA 사용](https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea)
1. [시스템에 설치된 볼륨 확인](#GetLogicalDrives)
2. [씨디롬](#GetDriveTypeA) & [ZEPHYR%d](#GetVolumeInformationA) 레이블의 디스크 검출
3. mciSendCommandA [MCI_OPEN](https://docs.microsoft.com/en-us/windows/win32/multimedia/mci-open), MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, [MCI_OPEN_PARMS](https://docs.microsoft.com/en-us/previous-versions/ms710954(v=vs.85))
    - mci 장비 검색
	- MCI_OPEN_PARMS->wDeviceID
4. mciSendCommandA [MCI_STATUS](https://docs.microsoft.com/en-us/windows/win32/multimedia/mci-status), MCI_STATUS_ITEM, [MCI_STATUS_PARMS](https://docs.microsoft.com/en-us/windows/win32/multimedia/mci-status-parms)
    - MCI_STATUS_PARMS->dwReturn 디스크 트랙수
5. mciSendCommandA MCI_STATUS, [MCI_STATUS_ITEM | MCI_TRACK](https://docs.microsoft.com/ko-kr/windows/win32/multimedia/retrieving-compact-disc-track-specific-information), MCI_STATUS_LENGTH
    - MCI_STATUS_PARMS->dwReturn dwParam2->dwTrack의 길이 
	- [MCI_MAKE_MSF](https://docs.microsoft.com/hr-hr/windows/win32/multimedia/mci-make-msf) 참조(또는 [MCI_MAKE_TMSF](https://docs.microsoft.com/en-us/previous-versions/ms710937(v=vs.85)))

## REGIONS 폴더 데이터
별다른 과정 없이 비교가 가능하다.

## MAP 폴더 데이터
[INDEO 4](https://en.wikipedia.org/wiki/Indeo)로 압축된 동영상 파일과 [ARJ](https://en.wikipedia.org/wiki/ARJ)로 압축된 동영상 파일이 존재합니다.
[ARJ 압축 파일 특성](https://docs.fileformat.com/compression/arj/)상 파일의 속성이 추가적으로 포함되어 있기에 단순 바이트 비교시 파일 내용이 다르게 결과가 도출됩니다.  
압축을 해제하면 정확한 비교가 가능하며, 다음과 같은 명령으를 명령 프롬프트에서 실행시 간단히 확장자를 변경할 수 있습니다.
(파일헤더를 arj, avi 파일 판단이 가능하지만 많은 파일이 아니므로 압축 해제를 시도합니다.) 
``` Command Prompt
ren *.zmk *.arj
```

### 참조
 - [ARJ 압축 파일 특성](https://docs.fileformat.com/compression/arj/)
 - [AVI 파일 특성](https://docs.microsoft.com/en-us/previous-versions//ms779636(v=vs.85)?redirectedfrom=MSDN)


## CD Audio 데이터
립핑툴에 따라 다르겠지만 하나의 디스크에서는 차이가 안나는게 정상이지만 같은 음악이더라도 다른 디스크의 음악은 립핑된 데이타 차이를 보일 수 있다.(아마 ODD에 따라 달라질 수 있다고 보지만... 자세하 내용은 관련 표준문서를 참고자 필요하다.)
기계적으로 패턴을 분석하고자 한다면 Audio Fingerprinting 또는 DNA 추출이라 하는 방법을 이용할 수 있다.  
- 참조문헌
    - [MS리서치](https://www.microsoft.com/en-us/research/publication/using-audio-fingerprinting-for-duplicate-detection-and-thumbnail-generation/)
    - https://github.com/spotify/echoprint-server
    - https://github.com/AddictedCS/soundfingerprinting
  
## 원본 폴더 구성
```
Install Root (1.2 패치 이후)
|- Chars
| | Chars.Zmk, Enemy.Enc
|- Data
| | *.bmp, *.fnt, *.pal, *.spr, *.cur
|- Objs
| | *.atc, *.itc, *.obi, *.sef, *.zmk
|- Regions
| | *.arc
| Zephyr.Exe
| Zephyr%02d.sav
```

## 추가 폴더 구성(디스크 통합을 위한...)
```
Install Root
|- Disk
| |- 1,2,3,Common
| | |- MAPS
| | | *.zmk
| | |- REGIONS
| | | *.arc, *.zrc
| |- BGM
| | | %02d,%02d,%02d.ogg
```

### BGM 파일 규칙 설명
- 1번CD위치,2번씨디위치,3번씨디위치.ogg
- 해당디스크에 없는 트렉이라면 00을 넣는다.

## CD 에뮬레이터 구조
### CDROM (CDFS 및 CDAUDIO 혼성)
- 초기 디스크 번호는 1
- 현재 필요한 디스크번호를 disk%d.Bmp 부터 제공 받음
- CD 오디오
    - 재생,정지,이동,길이,트랙수
- 볼륨레이블(명), 가상드라이브 여부, 파일 경로 리턴(CDFS 대리)
#### CDFS(HxD, Duplicate Files Finder, 반디집을 이용한 중복 제거)
- CreateFileA를 이용 실제 경로를 리턴
    - 추가 폴더 구성에서 아라비아 숫자로 구성된 폴더부터 우선 탐색하며 없다면 common애서 탐색
#### AUDIO(Foobar2000를 이용한 추출)
- 요청받은 오디오 트랙번호 기능 OGG 검색 및 재생
    - DirectSound 및 libvorbis 의존

## 추가 시험용 기능
- 키보드 이동 추가
    - 클릭 기능은 없으며 단순히 상하좌우로 움직임.
- 속도조절 기능
    - [timeGetTime](https://docs.microsoft.com/en-us/windows/win32/api/timeapi/nf-timeapi-timegettime)을 이용하여 게임속도(프레임)을 조절하는 것 같은데, 증감시는 괜찮으나 가감시에는 게임이 프리징되는 문제가 있음

## 추가 관심이 필요한 영역
- DirectDraw의 Sprite 처리 기법이나 메모리 구조를 확인하면 게임 내부 객체와 인터렉션이 가능.
- 속도조절 가감시 프리징 개선
- 치트키? 도 만들수 있을듯?

``` c++
/// https://www.gamedev.net/forums/topic/34089-why-is-directdraw-so-slow/
/// 함수 원형은 아래와 비슷할것으로 보임
DWORD Time1,Time2;
int fps=0;
HDC hdc; // just if not already declared
char buffer[10]; // just need for checking

while(!end)
{
	Time1=timeGetTime();
	Heartbeat(); // all your routines

	Time2=timeGetTime();
	fps=1000/(Time2-Time1); // calculate FPS
	if (lpDDsBack->GetDC(&hdc) == DD_OK) // give it out
	{
		SetTextColor(hdc, RGB( 255, 0, 0 ));
		itoa(fps, buffer, 10);
		TextOut(hdc, xpos, ypos, buffer, strlen(buffer)); // xpos, ypos = value of pos where to write FPS you have to fill it
		lpDDsBack->ReleaseDC(hdc);
	} //
	lpDDsPrimary->Flip(NULL,DDFLIP_WAIT);
}
```

## 사용한 오픈소스
- xiph.org [libogg](https://github.com/xiph/ogg) [`1.3.5`](https://downloads.xiph.org/releases/ogg/libogg-1.3.5.zip)
- xiph.org [libvorbis](https://github.com/xiph/vorbis) [`1.3.7`](https://downloads.xiph.org/releases/vorbis/libvorbis-1.3.7.zip)
- Microsoft [Detours](https://github.com/microsoft/Detours) [`v4.0.1`](https://github.com/microsoft/Detours/archive/refs/tags/v4.0.1.zip) 
- [Using DirectSound to Play Audio Stream Data](https://www.codeproject.com/Articles/8396/Using-DirectSound-to-Play-Audio-Stream-Data)
- [OGG & DirectSound](https://202psj.tistory.com/84)
- 간단히 창모드로 실행하기 위하여 [cnc-ddraw](https://github.com/CnCNet/cnc-ddraw)
  
## 분석 도구 및 그외 도구
- OllyDbg [`1.10`](https://www.ollydbg.de/)
  - 플러그인
    - (Ultra String Reference v0.12 ustrref.dll)
- [Rohitab](http://www.rohitab.com) API Monitor v2 (Alpha-r13) - x86 32-bit
- [Duplicate Files Finder](https://sourceforge.net/projects/doubles/) [`v0.8.0`](https://sourceforge.net/projects/doubles/files/Duplicate%20Files%20Finder/0.8.0/)
- [Foobar2000](https://www.foobar2000.org/download)
  - [Free Encoder Pack](https://www.foobar2000.org/download#:~:text=Free%20Encoder%20Pack%3A%20encoder%20binaries%20for%20the%20Converter%20component)
- [반디집](https://kr.bandisoft.com/bandizip/)
- [HxD](https://mh-nexus.de/en/hxd/)
- [샤나인코더](https://shana.pe.kr/)
