#version 430 core
#include "shadowSamples.glsl"

uniform sampler2D geometryPosWorld;
uniform sampler2D geometryAlbedo;
uniform sampler2D geometryNormal;
uniform sampler2D geometryDepth;

uniform mat4 viewProjection;
uniform vec3 directionalLightDir;

uniform sampler2D shadowMaps[16];
uniform mat4 worldToShadowMap[16];
uniform float cascadeZ[16];
uniform vec3 cascadeScale[16];
uniform int cascadeCount;
uniform mat4 worldToView;
uniform float farPlane;

// TODO: Interpolating in the arena between the cascades would reduce seams between them.
float shadow(vec3 posWorldSpace, vec3 normal)
{
    vec4 fragPosViewSpace = worldToView * vec4(posWorldSpace, 1.0);
    // This value should be negated or not based on the sign of the forward vector.
    // Just using abs makes it simpler becuase it works with any forward direction.
    float distanceFromView = abs(fragPosViewSpace.z);

    // If not found then use the furthest cascade.
    int layer = cascadeCount;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (distanceFromView < cascadeZ[i])
        {
            layer = i;
            break;
        }
    }
    float cosNormalLight = dot(normal, directionalLightDir);
    float sinNormalLight = sqrt(1.0 - cosNormalLight * cosNormalLight);
    float normalBiasScale = 0.087;
    posWorldSpace += normal * sinNormalLight * normalBiasScale;

    vec3 posLightSpace = (worldToShadowMap[layer] * vec4(posWorldSpace, 1.0)).xyz;
    posLightSpace = posLightSpace * 0.5 + 0.5;
    //posLightSpace *= vec3(0.5, 0.5, 1.0);
    //posLightSpace += vec3(0.5, 0.5, 0.5);

    float currentDepth = posLightSpace.z;

    if (currentDepth > 1.0)
    {
        return 0.0;
    }

    float constantBias = 8.09717e-5;

    // TODO: This value can get really big if the angle between the normal and the light is near zero. Set some max value for it.
    // TODO: This uses the geometry buffer normals not the surface normals. Either store an additional buffer with the surface normals or apply the effect in the shadow fragment shader.
    float tanNormalLight = sinNormalLight / cosNormalLight;
    float slopeBiasScale = -0.000161943;
    float slopeBias = tanNormalLight * slopeBiasScale;

    vec2 texelSize = 1.0 / vec2(textureSize(shadowMaps[layer], 0));
    float shadow = 0.0;
    for (int i = 0; i < shadowSamplesPositions.length(); i++)
    {
        vec2 sampleOffset = shadowSamplesPositions[i] * cascadeScale[layer].x * 20  * texelSize;
        float closestDepth = texture2D(shadowMaps[layer], posLightSpace.xy + sampleOffset).r;
        // Some articles suggest to scale the bias by the size of the cascade, but I couldn't get that to work well.
        shadow += (currentDepth - constantBias - slopeBias) > closestDepth ? 1.0 : 0.0;
    } 
    return shadow / shadowSamplesPositions.length();
}

// I don't think SSAO looks very good with simple scenes. On it's own it looks nice. but when applied to the final image it makes it look unrealistic. Also this implementation is quite noisy. To make it look better the SSAO would probably need to be rendered to a separate texture and blurred. The sample distribution could also be improved.
float ssao(vec3 posWorld, vec3 normal)
{
//    let r = ""
//    for (let i = 0; i < 91; i++) {
//      let ra = Math.sqrt(Math.random() * 1);
//      let theta = Math.random() * Math.PI * 2;
//      let phi = Math.random() * Math.PI * 2;
//      r += `vec3(${Math.cos(phi) * Math.sin(theta) * ra}, ${Math.sin(phi) * Math.sin(theta) * ra}, ${Math.cos(theta) * ra}), `
//    }
//    console.log(r);
    vec3 samples[] = {
        vec3(-0.42893075029370736, 0.1363660184940506, 0.21643254839860834), vec3(-0.11478492184155029, -0.1032833893432065, 0.48857866308935416), vec3(-0.5591210067079296, 0.15899869741953887, 0.7232187727609042), vec3(0.38492144312826004, 0.27423017767910063, 0.1285344368436948), vec3(0.00854067104339703, -0.09171491047612107, 0.5051117581592591), vec3(0.08762536696315938, -0.20466915324668977, -0.029984351915678952), vec3(-0.4456342747517679, -0.32826184027072214, -0.5093474014477315), vec3(-0.3431314302177717, 0.15765235785754714, 0.0981002603603576), vec3(0.49948172283562375, 0.6913966454910233, -0.24853935662461993), vec3(-0.03406428797311464, 0.000851661924474254, -0.15424103123198696), vec3(0.2952063621807305, -0.2640032527923883, 0.3380239272859342), vec3(-0.2718388701142402, 0.6083109527519756, 0.17534503987223193), vec3(0.5624232870559237, -0.16853430394373578, -0.7022735755462917), vec3(0.7757751337180205, -0.21843692999440764, -0.1733859207902116), vec3(0.2484565392091345, -0.35334486040492047, -0.38835955956171897), vec3(0.10829090372090075, -0.04859376081394193, 0.8803078388306468), vec3(-0.4481868796707748, -0.20619656141054948, -0.648934613139042), vec3(-0.10684094959745515, 0.3540649606565866, 0.5176424693035412), vec3(-0.05133889165035772, -0.0424837063165747, 0.23911382715484333), vec3(-0.06007448686958127, -0.07647484015792474, 0.7445963947901248), vec3(-0.08621802569313439, -0.019169071594854734, 0.14321336937575793), vec3(0.11993875246108938, 0.09081500853536895, 0.12250079776422544), vec3(-0.07430975748217024, -0.2792598858009151, -0.820217376866732), vec3(0.1838916477630944, 0.3504655365726218, 0.2931428520608943), vec3(0.9621504201636912, 0.031151542052643737, 0.1144077020275265), vec3(0.24953161412003777, 0.2816724526193373, 0.9225743292478881), vec3(0.010913787108049253, 0.05846119150171702, 0.7845533460557337), vec3(0.22269761520026293, -0.397264319344038, 0.1115591682815589), vec3(-0.10763367998990928, -0.23737637838219516, -0.9576641649076802), vec3(-0.15629635008871087, -0.7460149054335815, 0.12471992932218305), vec3(-0.06603147487630447, 0.11629080639636317, 0.406941199426253), vec3(-0.03414191852612693, 0.03763916070277755, -0.2875793232269158), vec3(0.8234497036481753, -0.25820949025540374, -0.08558832775852006), vec3(-0.1453627724891777, -0.1662691962216224, -0.20018711631509067), vec3(-0.01495186558851807, 0.009510058581820765, 0.8407120841994586), vec3(-0.26808035806223074, -0.07302708130078325, -0.9306177297186329), vec3(-0.08676314313632537, -0.3753681723076297, 0.5834960663982002), vec3(0.38986898014774796, -0.14158063571276105, -0.5819400141312977), vec3(0.2501442560751031, -0.5457375633678674, -0.26995101396653576), vec3(-0.36502107072401074, 0.18676387733286617, 0.7440269602514086), vec3(0.06154702638265171, -0.20830035464394306, 0.8979847772763357), vec3(0.0027278263920126828, 0.003324002406841249, -0.029034813054743862), vec3(0.27514433143790595, -0.308290837331736, 0.5349349100404561), vec3(-0.13949566338890743, -0.25775527581380364, 0.8646647756905751), vec3(-0.21869726302592843, -0.5670437467741837, 0.3731619075523533), vec3(-0.3229859864768471, 0.22777517206553835, -0.6219285370865071), vec3(0.01461627430581403, -0.031927502987471525, 0.3149305746068862), vec3(-0.2677684836009794, 0.4626320715260663, -0.7078630321032439), vec3(-0.32797842562803536, -0.22679708524131323, -0.38791464713306684), vec3(0.4069320825120693, 0.2677056005929808, 0.172099140919328), vec3(0.06904546086624734, -0.5579395405269268, 0.09971600914731364), vec3(0.019851246881633616, -0.011234613315911147, -0.5306108555221116), vec3(0.16102331448036575, -0.701421891804024, 0.3156669895500189), vec3(-0.40266528076898345, 0.33669326864581217, 0.3059129560503246), vec3(-0.8391640944646421, 0.06880352229123635, 0.14490832480682206), vec3(-0.6432252386602473, -0.5137093599033729, 0.5219704267012479), vec3(0.09609579846412408, 0.10389757468497848, -0.33867493849133695), vec3(-0.17944476741656323, -0.07234852094399005, -0.705232363516331), vec3(0.8558768043591222, 0.19737860633593135, -0.2836820254126053), vec3(-0.5799837956779607, -0.18360015333739252, 0.08327238602476202), vec3(-0.07267336718095299, 0.043045816917474375, 0.168143607622342), vec3(0.10742773442787475, -0.7547619310663698, 0.14976944292755434), vec3(0.3803626876464107, 0.027191983360829047, -0.33145090330658583), vec3(0.3890646169799677, 0.06536328068072603, -0.6278169173809969), vec3(-0.14603840715699334, 0.22249720313412746, 0.4566823767157665), vec3(-0.06758433840929809, -0.025656952009837142, -0.5629029311034028), vec3(0.2795717382485307, -0.05530994493979547, 0.5083230541287987), vec3(0.057039803575963666, -0.26751849264045036, -0.52199230209257), vec3(-0.010814137764322971, 0.011969966325787725, -0.1474753656886635), vec3(0.005510903774119839, -0.0011197274198499981, -0.0047796523357337695), vec3(-0.09686497466201498, 0.03319744493486204, 0.3980618283112388), vec3(0.3375882700643029, -0.5551763154468649, -0.22931709977008888), vec3(-0.05660446010610352, 0.32368439016145284, -0.5099460305355226), vec3(-0.20141692474260087, 0.241495149036699, -0.42861918258982484), vec3(-0.005195589552347236, 0.01969462763651644, 0.723161797081366), vec3(-0.8495750802957203, 0.44428648922943376, -0.0867742316647453), vec3(0.21098749374220296, 0.4163071132761005, 0.7685981947137203), vec3(-0.14611738461435772, 0.4417549285592164, -0.25903102061177047), vec3(0.3319906826398836, -0.7511130554953196, 0.17392148877403563), vec3(-0.49510704746604767, -0.431810102485011, -0.1549717468963549), vec3(0.42551097145951894, 0.02862022830432666, 0.22286357134039922), vec3(0.004801900711654419, 0.003236279244322678, 0.6236383606534515), vec3(-0.39210445580130604, 0.08503415846394358, -0.4971777348282948), vec3(0.2812680172150884, -0.43139218116843614, 0.39286944585344846), vec3(-0.5851696401437175, 0.49336323560298606, -0.17032507420747453), vec3(0.154811469150597, -0.8140164024137494, -0.06987013798183826), vec3(0.40144990693955646, 0.6076981232493234, 0.4552968577630134), vec3(-0.6803287024273724, 0.03733664546575318, 0.610522616975036), vec3(0.3801936904937732, 0.5545375812228743, -0.72724806741484), vec3(-0.4178478522911237, -0.6492120986649936, -0.2930040970731977), vec3(0.0780773276773006, -0.037109380401149174, -0.8355591071437626), 
    };

    float occlusion = 0.0;
    for (int i = 0; i < samples.length(); i++)
    {   
        if (dot(samples[i], normal) < 0.0)
            continue;

        vec3 samplePosWorld = posWorld + samples[i] * 0.1;
        
        vec4 samplePosNdc = viewProjection * vec4(samplePosWorld, 1.0);
        vec3 samplePos = samplePosNdc.xyz / samplePosNdc.w;
        float sampleDepth = samplePosNdc.z / samplePosNdc.w;
        if (sampleDepth > texture(geometryDepth, (samplePos.xy + 1) / 2).r * 2 - 1)
            occlusion += 1.0;
    }
    return 1.0 - (occlusion / samples.length());
}

in vec2 texturePos;

out vec4 outColor;

void main() 
{
    vec3 color = texture2D(geometryAlbedo, texturePos).rgb;
    vec3 posWorld = texture2D(geometryPosWorld, texturePos).xyz;
    vec3 normal = normalize(texture(geometryNormal, texturePos).xyz); 
    float depth = texture2D(geometryDepth, texturePos).r;

    float shadow = shadow(posWorld, normal);
    float ambient = 0.3;
    float diffuse = clamp(dot(normal, -directionalLightDir), 0, 1);
    float specular = clamp(pow(dot(reflect(-directionalLightDir, normal), normal), 1), 0, 1);
    color *= ambient + ((1 - shadow) * diffuse);
    outColor = vec4(color, 1);
    gl_FragDepth = depth;
}