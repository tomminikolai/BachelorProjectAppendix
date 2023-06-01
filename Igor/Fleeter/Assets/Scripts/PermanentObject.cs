using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PermanentObject : MonoBehaviour
{
    void Awake()
    {
        DontDestroyOnLoad(gameObject);
    }
}
