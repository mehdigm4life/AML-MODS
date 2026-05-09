# SAMP AML Menu Mod

سكريبت **AML (Android Mod Loader)** للعبة **SA-MP / GTA San Andreas Multiplayer** على الأندرويد، مع قائمة تفاعلية متعددة الميزات. يُبنى تلقائياً عبر **GitHub Actions** إلى ملف `.so` جاهز للتثبيت.

> ⚠️ **للأغراض التعليمية فقط.** استخدام هذا السكريبت على سيرفرات SA-MP العامة يؤدي إلى الحظر الدائم.

---

## 🚀 طريقة البناء (Build)

### الخيار 1: GitHub Actions (الأسهل - بدون أي إعداد محلي)
1. ارفع المشروع كاملاً إلى مستودع GitHub (Repository) جديد.
2. اذهب إلى تبويب **Actions** في صفحة المستودع.
3. سيبدأ workflow بإسم **"Build SAMP AML Mod"** تلقائياً، أو اضغط **Run workflow** يدوياً.
4. بعد انتهاء البناء (3-5 دقائق)، نزل ملف `.so` من قسم **Artifacts**:
   - `SAMPAMLMenu-arm64-v8a` ← للأجهزة الحديثة (64-bit)
   - `SAMPAMLMenu-armeabi-v7a` ← للأجهزة القديمة (32-bit)
   - `SAMPAMLMenu-Release` ← الاثنان معاً مع تسمية واضحة

### الخيار 2: البناء محلياً
يتطلب تثبيت **Android NDK r25c** أو أحدث:
```bash
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk -j$(nproc)
```
الناتج في `libs/arm64-v8a/libSAMPAMLMenu.so` و `libs/armeabi-v7a/libSAMPAMLMenu.so`.

---

## 📲 التثبيت داخل اللعبة

### المتطلبات
- لعبة **GTA San Andreas** للأندرويد (نسخة 2.00 أو 2.10).
- **AML Loader** مثبّت — حمّله من: <https://github.com/TheOfficialFloW/AndroidModLoader>
- سكريبت **SA-MP** للأندرويد (`libsamp.so`).

### خطوات التركيب
1. حمّل ملف `.so` المناسب من **Actions → Artifacts**.
2. أعد تسميته إلى `libSAMPAMLMenu.so` (إذا لزم).
3. ضعه في:
   ```
   /storage/emulated/0/Android/data/com.rockstargames.gtasa/files/AML/
   ```
   أو (إذا كانت اللعبة معدّلة بطريقة مختلفة):
   ```
   /sdcard/Android/data/com.rockstargames.gtasa/files/mods/
   ```
4. شغّل GTA SA → ادخل سيرفر SA-MP.
5. **اضغط زر VOLUME_UP** لفتح أو إغلاق القائمة.
6. اضغط على عناصر القائمة باللمس لتفعيلها.

### تشخيص الأخطاء
ملف اللوغ تجده في:
```
/sdcard/Android/data/com.rockstargames.gtasa/files/AML.log
```
ابحث عن السطر `SAMP-AML-Menu` لمتابعة عمل السكريبت.

---

## 🛠️ الميزات (Features)

| القسم | الميزات |
|------|---------|
| **Hacks** | Invisible · Godmode · Infinite Ammo · No Reload · Speed Hack · Jump Hack · No Fall Damage · Wallhack · Auto Aim · Night Vision · Freeze Time |
| **Spawn Vehicles** | Infernus · Bullet · Turismo · NRG-500 · Sultan · Hydra · Hunter · Rhino Tank · Sanchez · PCJ-600 |
| **Money** | $1M · $10M · $100M |
| **Teleport** | Los Santos · San Fierro · Las Venturas · Area 51 · Mount Chiliad |
| **Weapons** | Desert Eagle · AK-47 · M4 · Sniper · RPG · Minigun |
| **World** | Heal Full · Clear/Max Wanted · Time (Noon/Midnight) · Weather (Sunny/Storm) |

---

## ⚙️ تخصيص العناوين (Offsets)

العناوين المستخدمة في `main/mod.cpp` و `main/main.cpp` (مثل `g_GTASA + 0x4FC4D8`) **placeholders** ويجب تحديثها حسب نسخة لعبتك:
- استخدم **IDA Pro** أو **Ghidra** لفتح `libGTASA.so` المستخرجة من APK.
- ابحث عن الدوال:
  - `FindPlayerPed`
  - `CCarCtrl::CreateCarForScript`
  - `CPlayerPed::SetPosition`
  - `CStreaming::RequestModel`
- استبدل العناوين/الرموز في الكود وأعد البناء عبر Actions.

---

## 📁 هيكل المشروع
```
SAMP-AML-Mod/
├── .github/workflows/build.yml   ← GitHub Actions CI
├── main/
│   ├── main.cpp                  ← نقطة الدخول + hooks
│   ├── menu.cpp / menu.h         ← القائمة وعرضها
│   ├── mod.cpp / mod.h           ← منطق الميزات
├── includes/mod/
│   ├── amlmod.h                  ← AML SDK headers (مختصر)
│   ├── logger.h
│   └── config.h
├── Android.mk
├── Application.mk
└── README.md
```

---

## 📜 الترخيص
MIT — للأغراض التعليمية فقط. لست مسؤولاً عن أي استخدام مسيء.
