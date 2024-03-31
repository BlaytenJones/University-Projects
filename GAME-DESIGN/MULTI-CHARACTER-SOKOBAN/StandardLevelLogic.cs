using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StandardLevelLogic : MonoBehaviour
{
    void Start()
    {
        GameManager.Instance.Parse();
        GameManager.Instance.buttCondition = false;
    }
}
