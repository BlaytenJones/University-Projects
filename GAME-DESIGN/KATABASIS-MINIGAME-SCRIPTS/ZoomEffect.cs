using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ZoomEffect : MonoBehaviour
{
    private float zoom = 7.3f;

    // Update is called once per frame
    void Update()
    {
        transform.localScale = new Vector3(zoom, zoom, 1);
        zoom -= .009f;
        if(zoom < 0)
        {
            Destroy(gameObject);
        }
    }
}
