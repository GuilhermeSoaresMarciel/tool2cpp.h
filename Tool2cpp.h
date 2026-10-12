#pragma once

#include <dlfcn.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <cstring>
#include <codecvt>
#include <locale>
#include <cstdint>

#define Tool2cpp(offset, ptr, orig) hook((void *)offset, (void *)ptr, (void **)&orig)

struct Il2CppClass
{
};
struct Il2CppMethod
{
    void *methodPointer;
};
struct Il2CppImage
{
};
struct Il2CppAssembly
{
};
struct Il2CppDomain
{
};
typedef void *Il2CppObject;
struct Il2CppString;
struct FieldInfo
{
};

using class_from_name_t = Il2CppClass *(*)(const Il2CppImage *, const char *, const char *);
using class_get_method_from_name_t = Il2CppMethod *(*)(Il2CppClass *, const char *, int);
using class_get_methods_t = void *(*)(Il2CppClass *, void **);
using domain_get_assemblies_t = const Il2CppAssembly **(*)(const Il2CppDomain *, size_t *);
using domain_get_t = Il2CppDomain *(*)();
using assembly_get_image_t = const Il2CppImage *(*)(const Il2CppAssembly *);
using object_new_t = void *(*)(Il2CppClass *);
using runtime_object_init_t = void (*)(void *);
using string_new_t = Il2CppString *(*)(const char *);
using class_get_field_from_name_t = FieldInfo *(*)(Il2CppClass *, const char *);
using field_get_offset_t = size_t (*)(FieldInfo *);
using string_chars_t = void *(*)(Il2CppString *);
using object_get_class_t = Il2CppClass *(*)(Il2CppObject *);
using class_get_name_t = const char *(*)(Il2CppClass *);
using class_get_namespace_t = const char *(*)(Il2CppClass *);
using field_static_get_value_t = void (*)(FieldInfo *, void *);
using field_static_set_value_t = void (*)(FieldInfo *, void *);
using array_new_t = Il2CppObject *(*)(Il2CppClass *, size_t);
using array_length_t = size_t (*)(Il2CppObject *);
using array_get_t = void *(*)(Il2CppObject *, size_t);
using array_set_t = void (*)(Il2CppObject *, size_t, void *);
using method_get_name_t = const char *(*)(Il2CppMethod *);
using method_get_param_count_t = int (*)(Il2CppMethod *);

static string_chars_t str_chars_ptr = nullptr;
static object_get_class_t obj_get_class = nullptr;
static class_get_name_t cls_get_name = nullptr;
static class_get_namespace_t cls_get_ns = nullptr;
static field_static_get_value_t field_static_get_value = nullptr;
static field_static_set_value_t field_static_set_value = nullptr;
static array_new_t il2cpp_array_new_ptr = nullptr;
static array_length_t il2cpp_array_length_ptr = nullptr;
static array_get_t il2cpp_array_get_ptr = nullptr;
static array_set_t il2cpp_array_set_ptr = nullptr;
static class_get_methods_t cls_get_methods_ptr = nullptr;
static method_get_name_t method_get_name_ptr = nullptr;
static method_get_param_count_t method_get_param_count_ptr = nullptr;

namespace GC
{
    void Init(void *h);
    void Keep(Il2CppObject *o);
    std::vector<Il2CppObject *> Find(Il2CppClass *k);
}

class ClassSearch
{
    static void *h;
    static domain_get_t dom_get;
    static domain_get_assemblies_t dom_get_asms;
    static assembly_get_image_t asm_get_img;
    static class_from_name_t cls_from_name;
    static class_get_method_from_name_t cls_get_mth;
    static string_new_t str_new;
    static class_get_field_from_name_t cls_get_field;
    static field_get_offset_t fld_get_off;
    static bool init;

    static void Init()
    {
        if (init)
            return;
        h = dlopen("libil2cpp.so", RTLD_NOW);
        if (!h)
            return;
        dom_get = (domain_get_t)dlsym(h, "il2cpp_domain_get");
        dom_get_asms = (domain_get_assemblies_t)dlsym(h, "il2cpp_domain_get_assemblies");
        asm_get_img = (assembly_get_image_t)dlsym(h, "il2cpp_assembly_get_image");
        cls_from_name = (class_from_name_t)dlsym(h, "il2cpp_class_from_name");
        cls_get_mth = (class_get_method_from_name_t)dlsym(h, "il2cpp_class_get_method_from_name");
        str_new = (string_new_t)dlsym(h, "il2cpp_string_new");
        str_chars_ptr = (string_chars_t)dlsym(h, "il2cpp_string_chars");
        cls_get_field = (class_get_field_from_name_t)dlsym(h, "il2cpp_class_get_field_from_name");
        fld_get_off = (field_get_offset_t)dlsym(h, "il2cpp_field_get_offset");
        obj_get_class = (object_get_class_t)dlsym(h, "il2cpp_object_get_class");
        cls_get_name = (class_get_name_t)dlsym(h, "il2cpp_class_get_name");
        cls_get_ns = (class_get_namespace_t)dlsym(h, "il2cpp_class_get_namespace");
        field_static_get_value = (field_static_get_value_t)dlsym(h, "il2cpp_field_static_get_value");
        field_static_set_value = (field_static_set_value_t)dlsym(h, "il2cpp_field_static_set_value");
        il2cpp_array_new_ptr = (array_new_t)dlsym(h, "il2cpp_array_new");
        il2cpp_array_length_ptr = (array_length_t)dlsym(h, "il2cpp_array_length");
        il2cpp_array_get_ptr = (array_get_t)dlsym(h, "il2cpp_array_get");
        il2cpp_array_set_ptr = (array_set_t)dlsym(h, "il2cpp_array_set");
        cls_get_methods_ptr = (class_get_methods_t)dlsym(h, "il2cpp_class_get_methods");
        method_get_name_ptr = (method_get_name_t)dlsym(h, "il2cpp_method_get_name");
        method_get_param_count_ptr = (method_get_param_count_t)dlsym(h, "il2cpp_method_get_param_count");
        GC::Init(h);
        init = true;
    }

public:
    Il2CppClass *k = nullptr;

    ClassSearch(const std::string &full)
    {
        Init();
        if (!h)
            return;

        // k = GetClass("", full.c_str());
        // if (k) return;

        auto pos = full.rfind('.');
        std::string ns = pos != std::string::npos ? full.substr(0, pos) : "";
        std::string cls = pos != std::string::npos ? full.substr(pos + 1) : full;
        k = GetClass(ns.c_str(), cls.c_str());
    }

    Il2CppClass *GetClass(const char *ns, const char *cls)
    {
        auto d = dom_get();
        if (!d)
            return nullptr;
        size_t n;
        auto asms = dom_get_asms(d, &n);
        for (size_t i = 0; i < n; ++i)
        {
            auto img = asm_get_img(asms[i]);
            if (!img)
                continue;
            auto klass = cls_from_name(img, ns, cls);
            if (klass)
                return klass;
        }
        return nullptr;
    }

    uintptr_t GetMethod(const char *name, int pc = -1) const
    {
        if (!k)
            return 0;
        if (cls_get_methods_ptr && method_get_name_ptr && method_get_param_count_ptr)
        {
            void *iter = nullptr;
            while (true)
            {
                auto m = (Il2CppMethod *)cls_get_methods_ptr(k, &iter);
                if (!m)
                    break;
                const char *mname = method_get_name_ptr(m);
                if (!mname)
                    continue;
                if (std::strcmp(mname, name) == 0)
                {
                    int count = method_get_param_count_ptr(m);
                    if (pc < 0 || pc == count)
                    {
                        if (m->methodPointer)
                            return reinterpret_cast<uintptr_t>(m->methodPointer);
                    }
                }
            }
        }
        if (pc >= 0)
        {
            auto m = cls_get_mth(k, name, pc);
            return m ? reinterpret_cast<uintptr_t>(m->methodPointer) : 0;
        }
        for (int i = 0; i <= 10; ++i)
        {
            auto m = cls_get_mth(k, name, i);
            if (m)
                return reinterpret_cast<uintptr_t>(m->methodPointer);
        }
        return 0;
    }

    Il2CppString *NewStr(const char *t) { return str_new(t); }
    FieldInfo *GetField(const char *name) { return k ? cls_get_field(k, name) : nullptr; }
    size_t GetFieldOff(const char *name)
    {
        auto f = GetField(name);
        return f ? fld_get_off(f) : 0;
    }
    friend class Tool2cpp;
};

void *ClassSearch::h = nullptr;
domain_get_t ClassSearch::dom_get = nullptr;
domain_get_assemblies_t ClassSearch::dom_get_asms = nullptr;
assembly_get_image_t ClassSearch::asm_get_img = nullptr;
class_from_name_t ClassSearch::cls_from_name = nullptr;
class_get_method_from_name_t ClassSearch::cls_get_mth = nullptr;
string_new_t ClassSearch::str_new = nullptr;
class_get_field_from_name_t ClassSearch::cls_get_field = nullptr;
field_get_offset_t ClassSearch::fld_get_off = nullptr;
bool ClassSearch::init = false;

class Tool2cpp
{
    static std::unordered_map<std::string, std::chrono::steady_clock::time_point> timers;
    static std::unordered_map<std::string, int> intervals;
    static bool init;

    static void KeepIfManaged(Il2CppObject *o)
    {
        if (o)
            GC::Keep(o);
    }
    template <typename T>
    static void KeepIfManaged(T) {}
    static size_t GetFieldOff(const char *cls, Il2CppObject *inst, const char *fld)
    {
        if (!inst)
            return 0;
        ClassSearch cs(cls);
        return cs.GetFieldOff(fld);
    }

public:
    static void Init() { init = true; }
    static bool IsInit() { return init; }

    struct Vector2
    {
        float X, Y;
    };
    struct Vector3
    {
        float x, y, z;

        Vector3 operator+(const Vector3 &rhs) const
        {
            return {x + rhs.x, y + rhs.y, z + rhs.z};
        }

        Vector3 operator-(const Vector3 &rhs) const
        {
            return {x - rhs.x, y - rhs.y, z - rhs.z};
        }

        Vector3 operator*(float scalar) const
        {
            return {x * scalar, y * scalar, z * scalar};
        }

        Vector3 normalized() const
        {
            float length = std::sqrt(x * x + y * y + z * z);
            if (length == 0)
                return {0, 0, 0};
            return {x / length, y / length, z / length};
        }
    };
    struct Quaternion
    {
        float a, b, c, d;
    };

    static std::string ObjClsName(Il2CppObject *o)
    {
        if (!o || !obj_get_class || !cls_get_name || !cls_get_ns)
            return "";
        Il2CppClass *k = obj_get_class(o);
        return std::string(cls_get_ns(k)) + "." + cls_get_name(k);
    }
    static const char *FindClass(const char *cls)
    {
        ClassSearch cs(cls);
        if (cs.k)
            return cls;
    }
    static uintptr_t FindMethod(const char *cls, const char *mth, int pc = -1)
    {
        ClassSearch cs(cls);
        return cs.GetMethod(mth, pc);
    }
    template <typename Ret, typename... Args>
    static Ret invoke_static_method(const char *cls, const char *mth, Args... args)
    {
        ClassSearch cs(cls);
        auto off = cs.GetMethod(mth, (int)sizeof...(args));
        if (!off)
            off = cs.GetMethod(mth, -1);
        return off ? reinterpret_cast<Ret (*)(Args...)>(off)(args...) : Ret();
    }
    template <typename Ret, typename... Args>
    static Ret invoke_method(Il2CppObject *inst, const char *cls, const char *mth, Args... args)
    {
        ClassSearch cs(cls);
        auto off = cs.GetMethod(mth, (int)sizeof...(args));
        if (!off)
            off = cs.GetMethod(mth, -1);
        return off ? reinterpret_cast<Ret (*)(void *, Args...)>(off)(inst, args...) : Ret();
    }
    static Il2CppString *NewString(const char *t)
    {
        return ClassSearch("").NewStr(t);
    }
    static void *create_Obj(const char *cls)
    {
        ClassSearch cs(cls);
        if (!cs.k)
            return nullptr;
        static object_new_t obj_new = (object_new_t)dlsym(ClassSearch::h, "il2cpp_object_new");
        static runtime_object_init_t obj_init = (runtime_object_init_t)dlsym(ClassSearch::h, "il2cpp_runtime_object_init");
        if (!obj_new)
            return nullptr;
        void *inst = obj_new(cs.k);
        if (inst && obj_init)
            obj_init(inst);
        return inst;
    }
    static Il2CppObject *get_static_Obj(const char *cls, const char *mth, int pc = 0)
    {
        auto off = FindMethod(cls, mth, pc);
        if (!off)
            return nullptr;
        auto func = reinterpret_cast<Il2CppObject *(*)()>(off);
        return func();
    }
    template <typename T>
    static T getField(Il2CppObject *inst, const char *cls, const char *fld)
    {
        auto off = GetFieldOff(cls, inst, fld);
        T val = *reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(inst) + off);
        KeepIfManaged(val);
        return val;
    }
    template <typename T>
    static T get_static_Field(const char *cls, const char *fld)
    {
        ClassSearch cs(cls);
        FieldInfo *f = cs.GetField(fld);
        T val{};
        field_static_get_value(f, &val);
        KeepIfManaged(val);
        return val;
    }
    template <typename T>
    static void setField(Il2CppObject *inst, const char *cls, const char *fld, const T &v)
    {
        auto off = GetFieldOff(cls, inst, fld);
        std::memcpy(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(inst) + off), &v, sizeof(T));
    }
    template <typename T>
    static void set_static_Field(const char *cls, const char *fld, const T &v)
    {
        ClassSearch cs(cls);
        FieldInfo *f = cs.GetField(fld);
        field_static_set_value(f, (void *)&v);
    }
    static std::string to_string(Il2CppString *str)
    {
        const char16_t *utf16 = reinterpret_cast<const char16_t *>(str_chars_ptr(str));
        size_t len = 0;
        while (utf16[len])
            ++len;
        std::u16string u16(utf16, len);
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
        return conv.to_bytes(u16);
    }
    static const char *to_char(Il2CppString *str)
    {
        static std::string temp;
        temp = Tool2cpp::to_string(str);
        return temp.c_str();
    }
    static bool Timer(const std::string &name, int sec)
    {
        auto now = std::chrono::steady_clock::now();
        if (timers.find(name) == timers.end())
        {
            timers[name] = now;
            intervals[name] = sec;
            return false;
        }
        if (std::chrono::duration_cast<std::chrono::seconds>(now - timers[name]).count() >= intervals[name])
        {
            timers[name] = now;
            return true;
        }
        return false;
    }
    static std::vector<Il2CppObject *> FindObjects(const char *cls)
    {
        ClassSearch cs(cls);
        return GC::Find(cs.k);
    }
    static Il2CppObject *Array_New(Il2CppClass *elementClass, size_t length)
    {
        return il2cpp_array_new_ptr(elementClass, length);
    }
    static size_t Array_Length(Il2CppObject *arr)
    {
        return il2cpp_array_length_ptr(arr);
    }
    template <typename T>
    static T Array_Get(Il2CppObject *arr, size_t index)
    {
        T val = (T)il2cpp_array_get_ptr(arr, index);
        KeepIfManaged(val);
        return val;
    }
    template <typename T>
    static void Array_Set(Il2CppObject *arr, size_t index, T value)
    {
        il2cpp_array_set_ptr(arr, index, (void *)value);
    }
    template <typename T>
    static void Set_Camera_Orientation(const char *Camera, Il2CppObject *target_1, Il2CppObject *target_2)
    {

        auto UnityEngine_Camera = Tool2cpp::invoke_static_method<Il2CppObject *>("UnityEngine.Camera", Camera);

        auto get_transform_Camera = Tool2cpp::get_transform(UnityEngine_Camera);

        if (target_1 == nullptr)
            target_1 = UnityEngine_Camera;

        auto get_transform_target_1 = Tool2cpp::get_transform(target_1);

        auto get_transform_target_2 = Tool2cpp::get_transform(target_2);

        if (!get_transform_Camera || !get_transform_target_1 || !get_transform_target_2)
            return;

        Tool2cpp::Vector3 get_position_target_1 = Tool2cpp::get_position(get_transform_target_1);

        Tool2cpp::Vector3 get_position_target_2 = Tool2cpp::get_position(get_transform_target_2);

        Tool2cpp::Vector3 direction = {
            get_position_target_2.x - get_position_target_1.x,
            get_position_target_2.y - get_position_target_1.y,
            get_position_target_2.z - get_position_target_1.z};

        Tool2cpp::set_forward(get_transform_Camera, direction);
    }
    static Il2CppString *ReadFileAllText(const char *path)
    {
        return Tool2cpp::invoke_static_method<Il2CppString *>("System.IO.File", "ReadAllText", Tool2cpp::NewString(path));
    }
    static void OpenURL(const char *text)
    {
        Tool2cpp::invoke_static_method<Il2CppObject *>("UnityEngine.Application", "OpenURL", Tool2cpp::NewString(text));
    }
    static void set_systemCopyBuffer(const char *text)
    {
        Tool2cpp::invoke_static_method<Il2CppObject *>("UnityEngine.GUIUtility", "set_systemCopyBuffer", Tool2cpp::NewString(text));
    }
    static void *get_transform(void *c)
    {
        auto offset = FindMethod("UnityEngine.Component", "get_transform", 0);
        return offset ? reinterpret_cast<void *(*)(void *)>(offset)(c) : nullptr;
    }
    static Vector3 get_position(void *t)
    {
        auto offset = FindMethod("UnityEngine.Transform", "get_position", 0);
        return offset ? reinterpret_cast<Vector3 (*)(void *)>(offset)(t) : Vector3{};
    }
    static void set_position(void *t, Vector3 p)
    {
        auto offset = FindMethod("UnityEngine.Transform", "set_position", 1);
        if (offset)
            reinterpret_cast<void (*)(void *, Vector3)>(offset)(t, p);
    }
    static Vector3 get_localScale(void *t)
    {
        auto offset = FindMethod("UnityEngine.Transform", "get_localScale", 0);
        return offset ? reinterpret_cast<Vector3 (*)(void *)>(offset)(t) : Vector3{};
    }
    static void set_localScale(void *t, Vector3 p)
    {
        auto offset = FindMethod("UnityEngine.Transform", "set_localScale", 1);
        if (offset)
            reinterpret_cast<void (*)(void *, Vector3)>(offset)(t, p);
    }
    static Vector3 get_forward(void *t)
    {
        auto offset = FindMethod("UnityEngine.Transform", "get_forward", 0);
        return offset ? reinterpret_cast<Vector3 (*)(void *)>(offset)(t) : Vector3{};
    }
    static void set_forward(void *t, Vector3 p)
    {
        auto offset = FindMethod("UnityEngine.Transform", "set_forward", 1);
        if (offset)
            reinterpret_cast<void (*)(void *, Vector3)>(offset)(t, p);
    }
    static void Log(const char *m_log)
    {
        std::string path = Tool2cpp::to_string(Tool2cpp::invoke_static_method<Il2CppString *>("UnityEngine.Application", "get_persistentDataPath")) + "/tool2cpp_log.txt";
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        char timeBuf[64];
        std::strftime(timeBuf, sizeof(timeBuf), "[%Y-%m-%d %H:%M:%S] ", std::localtime(&t));
        std::string logLine = std::string(timeBuf) + m_log + "\n";
        Tool2cpp::invoke_static_method<Il2CppObject *>("System.IO.File", "AppendAllText", Tool2cpp::NewString(path.c_str()), Tool2cpp::NewString(logLine.c_str()));
    }
    static bool set_timeScale(float m_scale, bool m_bool)
    {
        static float originalTimeScale = 1.0f;
        static bool timeModified = false;

        if (m_bool)
        {
            if (!timeModified)
            {
                originalTimeScale = Tool2cpp::invoke_static_method<float>("UnityEngine.Time", "get_timeScale");
                timeModified = true;
            }
            Tool2cpp::invoke_static_method<Il2CppObject *>("UnityEngine.Time", "set_timeScale", m_scale);
        }
        else
        {
            if (timeModified)
            {
                Tool2cpp::invoke_static_method<Il2CppObject *>("UnityEngine.Time", "set_timeScale", originalTimeScale);
                timeModified = false;
            }
        }

        return timeModified;
    }
};

std::unordered_map<std::string, std::chrono::steady_clock::time_point> Tool2cpp::timers;
std::unordered_map<std::string, int> Tool2cpp::intervals;
bool Tool2cpp::init = false;

namespace GC
{
    using stop_gc_t = void (*)();
    using start_gc_t = void (*)();
    using live_alloc_t = void *(*)(Il2CppClass *, int, void (*)(Il2CppObject **, int, void *), void *, void *(*)(void *, size_t, void *));
    using live_calc_t = void (*)(void *);
    using live_fin_t = void (*)(void *);
    using live_free_t = void (*)(void *);
    using alloc_t = void *(*)(size_t);
    using free_t = void (*)(void *);

    static stop_gc_t stop_gc = nullptr;
    static start_gc_t start_gc = nullptr;
    static live_alloc_t live_alloc = nullptr;
    static live_calc_t live_calc = nullptr;
    static live_fin_t live_fin = nullptr;
    static live_free_t live_free = nullptr;
    static alloc_t mem_alloc = nullptr;
    static free_t mem_free = nullptr;

    void Init(void *h)
    {
        stop_gc = (stop_gc_t)dlsym(h, "il2cpp_stop_gc_world");
        start_gc = (start_gc_t)dlsym(h, "il2cpp_start_gc_world");
        live_alloc = (live_alloc_t)dlsym(h, "il2cpp_unity_liveness_allocate_struct");
        live_calc = (live_calc_t)dlsym(h, "il2cpp_unity_liveness_calculation_from_statics");
        live_fin = (live_fin_t)dlsym(h, "il2cpp_unity_liveness_finalize");
        live_free = (live_free_t)dlsym(h, "il2cpp_unity_liveness_free_struct");
        mem_alloc = (alloc_t)dlsym(h, "il2cpp_alloc");
        mem_free = (free_t)dlsym(h, "il2cpp_free");
    }

    std::vector<Il2CppObject *> Find(Il2CppClass *k)
    {
        std::vector<Il2CppObject *> objs;
        auto cb = [](Il2CppObject **arr, int sz, void *ud)
        {
            auto vec = reinterpret_cast<std::vector<Il2CppObject *> *>(ud);
            vec->insert(vec->end(), arr, arr + sz);
        };
        auto realloc = [](void *ptr, size_t sz, void *) -> void *
        {
            if (ptr && !sz)
            {
                mem_free(ptr);
                return nullptr;
            }
            return mem_alloc(sz);
        };
        stop_gc();
        void *state = live_alloc(k, 0, cb, &objs, realloc);
        live_calc(state);
        live_fin(state);
        start_gc();
        live_free(state);
        for (auto o : objs)
            Keep(o);
        return objs;
    }

    void Keep(Il2CppObject *o)
    {
        Tool2cpp::invoke_static_method<Il2CppObject *>("System.GC", "KeepAlive", o);
    }
}