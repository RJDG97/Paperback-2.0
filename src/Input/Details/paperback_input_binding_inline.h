//#pragma once
//
//// TODO - Shift to external binding file
//
//#ifndef PPB_INPUT_BINDING_INLINE
//#define PPB_INPUT_BINDING_INLINE
//
///****************************************************************************************
//                                ** BINDING ACTION **
//
//  Info                |   EntityInfo
//                      |
//  Archetype           |   Ptr To Archetype w/ Player Controller
//
//****************************************************************************************/
//
//#define BEGIN_INPUT_ACTION                                                          \
//    PPB_INLINE                                                                      \
//    void OnInputAction( const float& Dt ) noexcept                                  \
//    {                                                                               \
//                                                                                    
//#define END_INPUT_ACTION                                                            \
//    }                                                                               \
//
//
//
////-----------------------------------
////         Player Scripts
////-----------------------------------
//
//namespace paperback::input::binding
//{
//    //-----------------------------------
//    //     Entity Movement Binding
//    //-----------------------------------
//
//    BEGIN_BINDING_CONSTRUCT( Entity_Movement_Binding )
//        BEGIN_INPUT_ACTION
//
//            // TODO - Update Query Initialization To Constructor Call
//            tools::query Query;
//            Query.m_Must.AddFromComponents< rigidforce, rotation, mass, player_controller >();
//		    Query.m_NoneOf.AddFromComponents<prefab>();
//
//            m_Coordinator.ForEach( m_Coordinator.Search( Query ), [&]( rigidforce& RF, rotation& Rot, player_controller& Controller )
//            {
//                std::cout << "W Pressed" << std::endl;
//            });
//
//        END_INPUT_ACTION
//    END_BINDING_CONSTRUCT
//}
//
//#endif