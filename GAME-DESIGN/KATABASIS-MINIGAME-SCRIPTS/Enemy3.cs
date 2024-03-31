using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy3 : MonoBehaviour
{
    Rigidbody enemy;
    public float pullForce = .7f;
    private float currPull = 0f;
    private float zValue;
    private int timer;

    // Start is called before the first frame update
    void Start()
    {
        enemy = GetComponent<Rigidbody>();
        zValue = transform.position.z;
        StartCoroutine(decay());
    }

    // Update is called once per frame
    void Update()
    {
        if (currPull < pullForce)
        {
            currPull += .005f;
        }
        Vector3 pullDir = (new Vector3(0, 1, zValue) - transform.position);
        enemy.AddForce(((pullDir * currPull)) * Time.deltaTime, ForceMode.Impulse);
    }

    IEnumerator decay()
    {
        yield return new WaitForSeconds(1f);
        timer++;
        if (timer > 7)
        {
            Destroy(gameObject);
        }
        StartCoroutine(decay());
    }
}
